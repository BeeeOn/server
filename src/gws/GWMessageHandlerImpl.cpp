#include <Poco/Logger.h>
#include <Poco/JSON/Array.h>

#include "di/Injectable.h"
#include "gwmessage/GWMessageType.h"
#include "gwmessage/GWRequest.h"
#include "gwmessage/GWLastValueResponse.h"
#include "gwmessage/GWDeviceListResponse.h"
#include "gwmessage/GWAck.h"
#include "gws/GWMessageHandlerImpl.h"
#include "model/DeviceDescription.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GWMessageHandlerImpl)
BEEEON_OBJECT_CASTABLE(GWMessageHandler)
BEEEON_OBJECT_PROPERTY("gatewayCommunicator", &GWMessageHandlerImpl::setGatewayCommunicator)
BEEEON_OBJECT_PROPERTY("responseExpectedQueue", &GWMessageHandlerImpl::setGWResponseExpectedQueue)
BEEEON_OBJECT_PROPERTY("rpcForwarder", &GWMessageHandlerImpl::setRPCForwarder)
BEEEON_OBJECT_PROPERTY("deviceService", &GWMessageHandlerImpl::setDeviceService)
BEEEON_OBJECT_PROPERTY("gatewayService", &GWMessageHandlerImpl::setGatewayService)
BEEEON_OBJECT_PROPERTY("sensorHistoryService", &GWMessageHandlerImpl::setSensorHistoryService)
BEEEON_OBJECT_PROPERTY("eventsExecutor", &GWMessageHandlerImpl::setEventsExecutor)
BEEEON_OBJECT_PROPERTY("dataListeners", &GWMessageHandlerImpl::registerDataListener)
BEEEON_OBJECT_PROPERTY("deviceListeners", &GWMessageHandlerImpl::registerDeviceListener)
BEEEON_OBJECT_HOOK("cleanup", &GWMessageHandlerImpl::cleanup)
BEEEON_OBJECT_END(BeeeOn, GWMessageHandlerImpl)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

void GWMessageHandlerImpl::handle(const GWMessage::Ptr message,
		const GatewayID &gatewayID)
{
	if (!message.cast<GWRequest>().isNull())
		handleRequest(message.cast<GWRequest>(), gatewayID);
	else if (!message.cast<GWResponse>().isNull())
		handleResponse(message.cast<GWResponse>(), gatewayID);
	else if (!message.cast<GWSensorDataExport>().isNull())
		handleSensorData(message.cast<GWSensorDataExport>(), gatewayID);
	else
		throw InvalidArgumentException("bad message type " + message->type());
}

void GWMessageHandlerImpl::handleRequest(GWRequest::Ptr request,
		const GatewayID &gatewayID)
{
	GWResponse::Ptr response;

	switch(request->type()) {
	case GWMessageType::NEW_DEVICE_REQUEST:
		response = handleNewDevice(request.cast<GWNewDeviceRequest>(), gatewayID);
		break;
	case GWMessageType::NEW_DEVICE_GROUP_REQUEST:
		response = handleNewDeviceGroup(request.cast<GWNewDeviceGroupRequest>(), gatewayID);
		break;
	case GWMessageType::LAST_VALUE_REQUEST:
		response = handleLastValue(request.cast<GWLastValueRequest>(), gatewayID);
		break;
	case GWMessageType::DEVICE_LIST_REQUEST:
		response = handleDeviceList(request.cast<GWDeviceListRequest>(), gatewayID);
		break;
	case GWMessageType::NOTICE_REQUEST:
		response = handleNotice(request.cast<GWNoticeRequest>(), gatewayID);
		break;
	default:
		throw InvalidArgumentException(
			"bad request type " + request->type());
	}

	try {
		m_gatewayCommunicator->sendMessage(gatewayID, response);
	}
	catch (const NotFoundException &e) {
		logger().log(e, __FILE__, __LINE__);
	}
}

void GWMessageHandlerImpl::handleResponse(GWResponse::Ptr response,
		const GatewayID &gatewayID)
{
	const GlobalID &responseID = response->id();

	m_responseExpectedQueue->notifyDelivered(gatewayID, responseID);
	m_rpcForwarder->forwardResponse(gatewayID, response);

	if (!response->ackExpected())
		return;

	try {
		m_gatewayCommunicator->sendMessage(gatewayID, response->ack());
	}
	catch (const NotFoundException &e) {
		logger().log(e, __FILE__, __LINE__);
	}
}

void GWMessageHandlerImpl::handleSensorData(GWSensorDataExport::Ptr dataExport,
		const GatewayID &gatewayID)
{
	GWSensorDataConfirm::Ptr dataConfirm = dataExport->confirm();

	vector<SensorData> data = dataExport->data();
	for (const auto &it : data) {
		Device device = it.deviceID();
		device.setGateway(gatewayID);

		vector<ModuleValue> moduleValues;
		for (const auto &sensorValue : it) {
			ModuleValue moduleValue;
			moduleValue.setModule(sensorValue.moduleID().value());

			if (sensorValue.isValid())
				moduleValue.setValue(sensorValue.value());
			else
				moduleValue.setValue(NAN);

			moduleValues.push_back(moduleValue);
		}

		try {
			m_sensorHistoryService->insertMany(
				device, it.timestamp(), moduleValues);

			if (moduleValues.empty())
				continue;

			SensorDataEvent e;
			e.setGatewayID(gatewayID);
			e.setDeviceID(device.id());
			e.setStamp(it.timestamp());
			e.setData(moduleValues);

			m_dataEventSource.fireEvent(e, &SensorDataListener::onReceived);
		}
		catch (const Exception &e) {
			logger().log(e, __FILE__, __LINE__);
		}
	}

	try {
		m_gatewayCommunicator->sendMessage(gatewayID, dataConfirm);
	}
	catch (const NotFoundException &e) {
		logger().log(e, __FILE__, __LINE__);
	}
}


GWResponse::Ptr GWMessageHandlerImpl::handleNewDevice(
		GWNewDeviceRequest::Ptr request, const GatewayID &gatewayID)
{
	GWResponse::Ptr response = request->derive();

	Device device(request->deviceID());
	device.setRefresh(request->deviceDescription().refreshTime());

	DeviceEvent event = {gatewayID, device.id()};

	try {

		if (m_deviceService->registerDevice(device,
				sanitizeDeviceDescription(request->deviceDescription()),
				gatewayID)) {
			response->setStatus(GWResponse::Status::SUCCESS);
			event.setName(device.name());
			event.setType(*device.type());
			m_deviceEventSource.fireEvent(event, &DeviceListener::onNewDevice);
		}
		else {
			response->setStatus(GWResponse::Status::FAILED);
			m_deviceEventSource.fireEvent(event, &DeviceListener::onRefusedNewDevice);
		}
	}
	BEEEON_CATCH_CHAIN_ACTION(logger(),
		response->setStatus(GWResponse::Status::FAILED);
		m_deviceEventSource.fireEvent(event, &DeviceListener::onRefusedNewDevice);
	);

	return response;
}

GWResponse::Ptr GWMessageHandlerImpl::handleNewDeviceGroup(
	GWNewDeviceGroupRequest::Ptr request, const GatewayID &gatewayID)
{
	GWResponse::Ptr response = request->derive();
	vector<DeviceDescription> descriptions;
	vector<DeviceEvent> events;

	for (const auto &des : request->deviceDescriptions()) {
		descriptions.emplace_back(sanitizeDeviceDescription(des));

		const DeviceEvent event = {gatewayID, des.id()};
		events.emplace_back(event);
	}

	try {
		m_deviceService->registerDeviceGroup(descriptions, gatewayID);

		response->setStatus(GWResponse::Status::SUCCESS);
		for (auto &event : events)
			m_deviceEventSource.fireEvent(event, &DeviceListener::onNewDevice);
	}
	BEEEON_CATCH_CHAIN_ACTION(logger(),
		response->setStatus(GWResponse::Status::FAILED);
		for (auto &event : events)
			m_deviceEventSource.fireEvent(event, &DeviceListener::onRefusedNewDevice);
	);

	return response;
}

GWResponse::Ptr GWMessageHandlerImpl::handleLastValue(
		GWLastValueRequest::Ptr request, const GatewayID &gatewayID)
{
	GWLastValueResponse::Ptr response =
		request->derive().cast<GWLastValueResponse>();

	Device device(request->deviceID());
	device.setGateway(gatewayID);

	ModuleInfo module;
	module.setId(request->moduleID().value());

	Timestamp timestamp;
	double value;

	try {
		if (m_sensorHistoryService->fetchLast(device, module, timestamp, value)) {
			response->setValid(!std::isnan(value));
			response->setValue(value);
			response->setStatus(GWResponse::Status::SUCCESS);
		}
		else {
			response->setStatus(GWResponse::Status::FAILED);
		}
	}
	BEEEON_CATCH_CHAIN_ACTION(logger(),
		response->setStatus(GWResponse::Status::FAILED);
	);

	return response;
}

GWResponse::Ptr GWMessageHandlerImpl::handleDeviceList(
		GWDeviceListRequest::Ptr request, const GatewayID &gatewayID)
{
	GWDeviceListResponse::Ptr response(
		request->derive().cast<GWDeviceListResponse>()
	);

	DevicePrefix prefix(request->devicePrefix());
	vector<DeviceWithData> devices;

	try {
		m_deviceService->fetchActiveWithPrefix(devices, gatewayID, prefix);

		vector<DeviceID> deviceIDs;
		for (const auto &device : devices)
			deviceIDs.push_back(device.id());

		response->setDevices(deviceIDs);

		for (const auto &device : devices) {
			size_t id = 0;
			map<ModuleID, double> values;

			for (const auto &value : device.values()) {
				if (value.isValid())
					values.emplace(id++, value.value());
			}

			if (values.empty())
				continue;

			response->setModulesValues(device.id(), values);
			response->setRefreshFor(device.id(), device.refresh());
		}

		response->setStatus(GWResponse::Status::SUCCESS);
	}
	BEEEON_CATCH_CHAIN_ACTION(logger(),
		response->setStatus(GWResponse::Status::FAILED);
	);

	return response;
}

GWResponse::Ptr GWMessageHandlerImpl::handleNotice(
		GWNoticeRequest::Ptr request,
		const GatewayID &gatewayID)
{
	GWResponse::Ptr response = request->derive();

	GatewayMessage message;
	message.setGateway({gatewayID});
	message.setAt(request->at());
	message.setSeverity(request->severity());
	message.setKey("gateway." + Sanitize::token(request->key()));

	auto context = request->context();
	vector<string> names;
	for (const auto &pair : *context)
		names.emplace_back(pair.first);

	// sanitize strings
	for (const auto &name : names) {
		const auto value = context->get(name);
		context->remove(name);

		if (value.isString()) {
			context->set(
				Sanitize::token(name),
				Sanitize::common(value.extract<string>()));
		}
		else if (value.isArray()) {
			JSON::Array::Ptr a = value.extract<JSON::Array::Ptr>();

			for (size_t i = 0; i < a->size(); ++i) {
				auto value = a->get(i);

				if (value.isString())
					a->set(i, Sanitize::common(value.extract<string>()));
			}

			context->set(Sanitize::token(name), a);
		}
		else {
			context->set(Sanitize::token(name), value);
		}
	}

	message.setContext(context);

	m_gatewayService->deliverMessage(message);

	return response;
}

DeviceDescription GWMessageHandlerImpl::sanitizeDeviceDescription(
		const DeviceDescription& description)
{
	DeviceDescription des;

	des.setID(description.id());
	des.setVendor(Sanitize::common(description.vendor()));
	des.setProductName(Sanitize::common(description.productName()));
	des.setDataTypes(description.dataTypes());
	des.setRefreshTime(description.refreshTime());

	return des;
}

void GWMessageHandlerImpl::cleanup()
{
	m_gatewayCommunicator = nullptr;
	m_responseExpectedQueue = nullptr;
	m_rpcForwarder = nullptr;
}

void GWMessageHandlerImpl::setGatewayCommunicator(
		GatewayCommunicator::Ptr communicator)
{
	m_gatewayCommunicator = communicator;
}

void GWMessageHandlerImpl::setGWResponseExpectedQueue(
	GWResponseExpectedQueue::Ptr queue)
{
	m_responseExpectedQueue = queue;
}

void GWMessageHandlerImpl::setRPCForwarder(RPCForwarder::Ptr forwarder)
{
	m_rpcForwarder = forwarder;
}

void GWMessageHandlerImpl::setDeviceService(GWSDeviceService::Ptr service)
{
	m_deviceService = service;
}

void GWMessageHandlerImpl::setGatewayService(GWSGatewayService::Ptr service)
{
	m_gatewayService = service;
}

void GWMessageHandlerImpl::setSensorHistoryService(
		GWSSensorHistoryService::Ptr service)
{
	m_sensorHistoryService = service;
}

void GWMessageHandlerImpl::registerDataListener(SensorDataListener::Ptr listener)
{
	m_dataEventSource.addListener(listener);
}

void GWMessageHandlerImpl::registerDeviceListener(DeviceListener::Ptr listener)
{
	m_deviceEventSource.addListener(listener);
}

void GWMessageHandlerImpl::setEventsExecutor(AsyncExecutor::Ptr executor)
{
	m_dataEventSource.setAsyncExecutor(executor);
	m_deviceEventSource.setAsyncExecutor(executor);
}
