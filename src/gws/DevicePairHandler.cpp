#include <Poco/Logger.h>
#include <Poco/Timestamp.h>

#include "gws/DevicePairHandler.h"

using namespace Poco;
using namespace BeeeOn;

DevicePairHandler::DevicePairHandler(
		const Device &device,
		DeviceDao::Ptr dao,
		SharedPtr<EventSource<DeviceListener>> source):
	m_device(device),
	m_deviceDao(dao),
	m_eventSource(source)
{
}

void DevicePairHandler::onPending(GatewayRPCResult::Ptr result)
{
	logger().information(
		"device " + m_device + " pairing is pending...",
		__FILE__, __LINE__);

	const DeviceEvent e = {m_device.gateway().id(), m_device.id()};
	m_eventSource->fireEvent(e, &DeviceListener::onPairRequested);
}

void DevicePairHandler::onAccepted(GatewayRPCResult::Ptr result)
{
	logger().debug(
		"device " + m_device + " would be paired",
		__FILE__, __LINE__);
}

void DevicePairHandler::onSuccess(GatewayRPCResult::Ptr result)
{
	logger().information(
		"device " + m_device + " successfully paired",
		__FILE__, __LINE__);

	m_device.status().setState(DeviceStatus::STATE_ACTIVE);
	m_device.status().setLastChanged(Timestamp());
	BEEEON_TRANSACTION(m_deviceDao->update(m_device, m_device.gateway()));

	const DeviceEvent e = {m_device.gateway().id(), m_device.id()};
	m_eventSource->fireEvent(e, &DeviceListener::onPairConfirmed);
}

void DevicePairHandler::onFailed(GatewayRPCResult::Ptr result)
{
	logger().warning(
		"device " + m_device + " failed to pair",
		__FILE__, __LINE__);

	m_device.status().setState(DeviceStatus::STATE_INACTIVE);
	m_device.status().setLastChanged(Timestamp());
	BEEEON_TRANSACTION(m_deviceDao->update(m_device, m_device.gateway()));

	const DeviceEvent e = {m_device.gateway().id(), m_device.id()};
	m_eventSource->fireEvent(e, &DeviceListener::onPairFailed);
}

void DevicePairHandler::onNotConnected(GatewayRPCResult::Ptr result)
{
	logger().warning(
		"device " + m_device + " failed to pair, gateway "
		+ m_device.gateway() + " not connected",
		__FILE__, __LINE__);

	m_device.status().setState(DeviceStatus::STATE_INACTIVE);
	m_device.status().setLastChanged(Timestamp());
	BEEEON_TRANSACTION(m_deviceDao->update(m_device, m_device.gateway()));

	const DeviceEvent e = {m_device.gateway().id(), m_device.id()};
	m_eventSource->fireEvent(e, &DeviceListener::onPairFailed);
}

void DevicePairHandler::onTimeout(GatewayRPCResult::Ptr result)
{
	logger().warning(
		"device " + m_device + " failed to pair on time",
		__FILE__, __LINE__);

	m_device.status().setState(DeviceStatus::STATE_INACTIVE);
	m_device.status().setLastChanged(Timestamp());
	BEEEON_TRANSACTION(m_deviceDao->update(m_device, m_device.gateway()));

	const DeviceEvent e = {m_device.gateway().id(), m_device.id()};
	m_eventSource->fireEvent(e, &DeviceListener::onPairFailed);
}
