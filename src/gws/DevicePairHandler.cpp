#include <Poco/Logger.h>
#include <Poco/Timestamp.h>

#include "gws/DevicePairHandler.h"

using namespace Poco;
using namespace BeeeOn;

DevicePairHandler::DevicePairHandler(const Device &device, DeviceDao::Ptr dao):
	m_device(device),
	m_deviceDao(dao)
{
}

void DevicePairHandler::onAny(GatewayRPCResult::Ptr r)
{
	Gateway gateway(m_device.gateway());

	switch (r->status()) {
	case GatewayRPCResult::Status::PENDING:
		logger().information(
			"device " + m_device + " pairing is pending...",
			__FILE__, __LINE__);
		break;

	case GatewayRPCResult::Status::ACCEPTED:
		logger().debug(
			"device " + m_device + " would be paired",
			__FILE__, __LINE__);
		break;

	case GatewayRPCResult::Status::SUCCESS:
		logger().information(
			"device " + m_device + " successfully paired",
			__FILE__, __LINE__);

		m_device.status().setState(DeviceStatus::STATE_ACTIVE);
		m_device.status().setLastChanged(Timestamp());
		BEEEON_TRANSACTION(m_deviceDao->update(m_device, gateway));
		break;

	case GatewayRPCResult::Status::FAILED:
		logger().warning(
			"device " + m_device + " failed to pair",
			__FILE__, __LINE__);

		m_device.status().setState(DeviceStatus::STATE_INACTIVE);
		m_device.status().setLastChanged(Timestamp());
		BEEEON_TRANSACTION(m_deviceDao->update(m_device, gateway));
		break;

	case GatewayRPCResult::Status::TIMEOUT:
	case GatewayRPCResult::Status::NOT_CONNECTED:
		logger().warning(
			"device " + m_device + " failed to pair on time",
			__FILE__, __LINE__);

		m_device.status().setState(DeviceStatus::STATE_INACTIVE);
		m_device.status().setLastChanged(Timestamp());
		BEEEON_TRANSACTION(m_deviceDao->update(m_device, gateway));
		break;
	}
}
