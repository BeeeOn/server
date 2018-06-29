#include <Poco/Logger.h>
#include <Poco/Timestamp.h>

#include "gws/DeviceUnpairHandler.h"

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

	case GatewayRPCResult::Status::SUCCESS:
		logger().information(
			"device " + m_device + " successfully unpaired",
			__FILE__, __LINE__);

		m_device.status().setState(DeviceStatus::STATE_INACTIVE);
		m_device.status().setLastChanged(Timestamp());
		BEEEON_TRANSACTION(m_deviceDao->update(m_device, gateway));
		m_eventSource->fireEvent(event, &DeviceListener::onUnpairConfirmed);
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
	}

}
