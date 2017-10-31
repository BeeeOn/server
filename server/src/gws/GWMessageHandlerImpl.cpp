#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "gwmessage/GWMessageType.h"
#include "gwmessage/GWRequest.h"
#include "gwmessage/GWLastValueResponse.h"
#include "gwmessage/GWDeviceListResponse.h"
#include "gwmessage/GWAck.h"
#include "gws/GWMessageHandlerImpl.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GWMessageHandlerImpl)
BEEEON_OBJECT_CASTABLE(GWMessageHandler)
BEEEON_OBJECT_REF("gatewayCommunicator", &GWMessageHandlerImpl::setGatewayCommunicator)
BEEEON_OBJECT_REF("responseExpectedQueue", &GWMessageHandlerImpl::setGWResponseExpectedQueue)
BEEEON_OBJECT_REF("rpcForwarder", &GWMessageHandlerImpl::setRPCForwarder)
BEEEON_OBJECT_REF("deviceService", &GWMessageHandlerImpl::setDeviceService)
BEEEON_OBJECT_REF("sensorHistoryService", &GWMessageHandlerImpl::setSensorHistoryService)
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
	case GWMessageType::LAST_VALUE_REQUEST:
		response = handleLastValue(request.cast<GWLastValueRequest>(), gatewayID);
		break;
	case GWMessageType::DEVICE_LIST_REQUEST:
		response = handleDeviceList(request.cast<GWDeviceListRequest>(), gatewayID);
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

	GWResponseWithAck::Ptr responseWithAck(response.cast<GWResponseWithAck>());
	if (responseWithAck.isNull())
		return;

	try {
		m_gatewayCommunicator->sendMessage(gatewayID, responseWithAck->ack());
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
	device.setRefresh(request->refreshTime());

	try {
		if (m_deviceService->registerDevice(device,
				request->productName(),
				request->vendor(),
				gatewayID)) {
			response->setStatus(GWResponse::Status::SUCCESS);
		}
		else {
			response->setStatus(GWResponse::Status::FAILED);
		}
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		response->setStatus(GWResponse::Status::FAILED);
	}
	catch (...) {
		logger().critical("unknown failure", __FILE__, __LINE__);
		response->setStatus(GWResponse::Status::FAILED);
	}

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
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		response->setStatus(GWResponse::Status::FAILED);
	}
	catch (...) {
		logger().critical("unknown failure", __FILE__, __LINE__);
		response->setStatus(GWResponse::Status::FAILED);
	}

	return response;
}

GWResponse::Ptr GWMessageHandlerImpl::handleDeviceList(
		GWDeviceListRequest::Ptr request, const GatewayID &gatewayID)
{
	GWDeviceListResponse::Ptr response(
		request->derive().cast<GWDeviceListResponse>()
	);

	DevicePrefix prefix(request->devicePrefix());
	vector<Device> devices;

	try {
		m_deviceService->fetchActiveWithPrefix(devices, gatewayID, prefix);

		vector<DeviceID> deviceIDs;
		for (const auto &device : devices)
			deviceIDs.push_back(device.id());

		response->setDevices(deviceIDs);
		response->setStatus(GWResponse::Status::SUCCESS);
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		response->setStatus(GWResponse::Status::FAILED);
	}
	catch (...) {
		logger().critical("unknown failure", __FILE__, __LINE__);
		response->setStatus(GWResponse::Status::FAILED);
	}

	return response;
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

void GWMessageHandlerImpl::setSensorHistoryService(
		GWSSensorHistoryService::Ptr service)
{
	m_sensorHistoryService = service;
}