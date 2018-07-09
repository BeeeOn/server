#include <Poco/Logger.h>
#include <Poco/Timestamp.h>

#include "gws/DeviceUnpairHandler.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DeviceUnpairHandler::DeviceUnpairHandler(
		const Device &device,
		DeviceDao::Ptr dao,
		SharedPtr<EventSource<DeviceListener>> source):
	m_device(device),
	m_deviceDao(dao),
	m_eventSource(source)
{
}

void DeviceUnpairHandler::deviceUnpaired(Device &device)
{
	const DeviceEvent event = {device.gateway().id(), device.id()};
	Gateway gateway(device.gateway());

	logger().information(
		"device " + device + " successfully unpaired",
		__FILE__, __LINE__);

	device.status().setState(DeviceStatus::STATE_INACTIVE);
	device.status().setLastChanged(Timestamp());
	BEEEON_TRANSACTION(m_deviceDao->update(device, gateway));
	m_eventSource->fireEvent(event, &DeviceListener::onUnpairConfirmed);
}

void DeviceUnpairHandler::onSuccess(GatewayRPCResult::Ptr r)
{
	GatewayRPCUnpairResult::Ptr result = r.cast<GatewayRPCUnpairResult>();
	const auto &unpaired = result.isNull() ? set<DeviceID>{} : result->unpaired();
	const bool foundRequested = unpaired.find(m_device.id()) != unpaired.end();

	// legacy response or response with empty unpaired set
	if ((unpaired.size() == 1 && foundRequested) || unpaired.empty()) {
		deviceUnpaired(m_device);
		return;
	}

	poco_assert(!unpaired.empty());

	// finialize unpair of the reported devices
	for (const auto &id : unpaired) {
		Device device(id);
		device.setGateway(m_device.gateway());

		deviceUnpaired(device);
	}

	// if the requested device has not been unpaired
	// we must notify about its failed unpair process
	if (!foundRequested) {
		const DeviceEvent event = {m_device.gateway().id(), m_device.id()};
		Gateway gateway(m_device.gateway());

		logger().warning(
			"different device(s) were unpaired instead of " + m_device,
			__FILE__, __LINE__);

		m_device.status().setState(DeviceStatus::STATE_ACTIVE);
		m_device.status().setLastChanged(Timestamp());
		BEEEON_TRANSACTION(m_deviceDao->update(m_device, gateway));
		m_eventSource->fireEvent(event, &DeviceListener::onUnpairFailed);
	}
}

void DeviceUnpairHandler::onAny(GatewayRPCResult::Ptr r)
{
	const DeviceEvent event = {m_device.gateway().id(), m_device.id()};
	Gateway gateway(m_device.gateway());

	switch (r->status()) {
	case GatewayRPCResult::Status::PENDING:
		logger().information(
			"device " + m_device + " unpairing is pending...",
			__FILE__, __LINE__);
		m_eventSource->fireEvent(event, &DeviceListener::onUnpairRequested);
		break;

	case GatewayRPCResult::Status::ACCEPTED:
		logger().debug(
			"device " + m_device + " would be unpaired",
			__FILE__, __LINE__);
		break;

	case GatewayRPCResult::Status::FAILED:
		logger().warning(
			"device " + m_device + " failed to unpair",
			__FILE__, __LINE__);

		m_device.status().setState(DeviceStatus::STATE_ACTIVE);
		m_device.status().setLastChanged(Timestamp());
		BEEEON_TRANSACTION(m_deviceDao->update(m_device, gateway));
		m_eventSource->fireEvent(event, &DeviceListener::onUnpairFailed);
		break;

	case GatewayRPCResult::Status::TIMEOUT:
		logger().warning(
			"device " + m_device + " failed to unpair on time",
			__FILE__, __LINE__);

		m_device.status().setState(DeviceStatus::STATE_ACTIVE);
		m_device.status().setLastChanged(Timestamp());
		BEEEON_TRANSACTION(m_deviceDao->update(m_device, gateway));
		m_eventSource->fireEvent(event, &DeviceListener::onUnpairFailed);
		break;

	case GatewayRPCResult::Status::NOT_CONNECTED:
		logger().warning(
			"device " + m_device + " failed to unpair, gateway "
			+ gateway + " not connected",
			__FILE__, __LINE__);

		m_device.status().setState(DeviceStatus::STATE_ACTIVE);
		m_device.status().setLastChanged(Timestamp());
		BEEEON_TRANSACTION(m_deviceDao->update(m_device, gateway));
		m_eventSource->fireEvent(event, &DeviceListener::onUnpairFailed);
		break;

	default:
		break;
	}

}
