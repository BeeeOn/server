#include <Poco/Logger.h>

#include "di/InjectorTarget.h"
#include "rpc/GatewayRPC.h"
#include "dao/DeviceDao.h"
#include "Debug.h"

using namespace Poco;

namespace BeeeOn {

/**
 * Fake RPC with no communication to Ada Server at all.
 */
class FakeGatewayRPC : public GatewayRPC, public AbstractInjectorTarget {
public:
	FakeGatewayRPC();

	void setDeviceDao(DeviceDao *dao);

	void sendListen(const Gateway &gateway) override;
	void unpairDevice(const Gateway &gateway,
			const Device &device) override;
	void pingGateway(const Gateway &gateway) override;

private:
	Logger &m_logger;
	DeviceDao *m_deviceDao;
};

FakeGatewayRPC::FakeGatewayRPC():
	m_logger(LOGGER_CLASS(this)),
	m_deviceDao(&NullDeviceDao::instance())
{
	injector<FakeGatewayRPC, DeviceDao>(
			"deviceDao",
			&FakeGatewayRPC::setDeviceDao
	);
}

void FakeGatewayRPC::setDeviceDao(DeviceDao *dao)
{
	m_deviceDao = dao? dao : &NullDeviceDao::instance();
}

void FakeGatewayRPC::sendListen(const Gateway &gateway)
{
	m_logger.warning("send listen to gateway",
			__FILE__, __LINE__);
}

void FakeGatewayRPC::unpairDevice(const Gateway &gateway,
		const Device &target)
{
	Device device(target);

	if (!m_deviceDao->fetch(device, gateway)) {
		m_logger.warning("failed to fetch device "
				+ device.id().toString(),
				__FILE__, __LINE__);
		return;
	}

	device.setActiveSince(Nullable<DateTime>());

	if (!m_deviceDao->update(device, gateway)) {
		m_logger.warning("failed to update device "
				+ device.id().toString(),
				__FILE__, __LINE__);
	}
}

void FakeGatewayRPC::pingGateway(const Gateway &gateway)
{
	m_logger.warning("ping gateway",
			__FILE__, __LINE__);
}

}

BEEEON_OBJECT(FakeGatewayRPC, BeeeOn::FakeGatewayRPC)
