#include <Poco/Logger.h>

#include "di/InjectorTarget.h"
#include "rpc/GatewayRPC.h"
#include "dao/DeviceDao.h"
#include "util/Loggable.h"

using namespace Poco;

namespace BeeeOn {

/**
 * Fake RPC with no communication to Ada Server at all.
 */
class FakeGatewayRPC : public GatewayRPC, public AbstractInjectorTarget,
		public Loggable {
public:
	FakeGatewayRPC();

	void setDeviceDao(DeviceDao *dao);

	void sendListen(const Gateway &gateway) override;
	void unpairDevice(const Gateway &gateway,
			const Device &device) override;
	void pingGateway(const Gateway &gateway) override;

private:
	DeviceDao *m_deviceDao;
};

FakeGatewayRPC::FakeGatewayRPC():
	m_deviceDao(&NullDeviceDao::instance())
{
	injector("deviceDao", &FakeGatewayRPC::setDeviceDao);
}

void FakeGatewayRPC::setDeviceDao(DeviceDao *dao)
{
	m_deviceDao = dao? dao : &NullDeviceDao::instance();
}

void FakeGatewayRPC::sendListen(const Gateway &gateway)
{
	logger().warning("send listen to gateway",
			__FILE__, __LINE__);
}

void FakeGatewayRPC::unpairDevice(const Gateway &gateway,
		const Device &target)
{
	Device device(target);

	if (!m_deviceDao->fetch(device, gateway)) {
		logger().warning("failed to fetch device "
				+ device.id().toString(),
				__FILE__, __LINE__);
		return;
	}

	device.setActiveSince(Nullable<DateTime>());

	if (!m_deviceDao->update(device, gateway)) {
		logger().warning("failed to update device "
				+ device.id().toString(),
				__FILE__, __LINE__);
	}
}

void FakeGatewayRPC::pingGateway(const Gateway &gateway)
{
	logger().warning("ping gateway",
			__FILE__, __LINE__);
}

}

BEEEON_OBJECT(FakeGatewayRPC, BeeeOn::FakeGatewayRPC)
