#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "rpc/GatewayRPC.h"
#include "dao/DeviceDao.h"
#include "util/Loggable.h"

using namespace Poco;

namespace BeeeOn {

/**
 * Fake RPC with no communication to Ada Server at all.
 */
class FakeGatewayRPC : public GatewayRPC,
		public Loggable {
public:
	FakeGatewayRPC();

	void setDeviceDao(DeviceDao::Ptr dao);

	void sendListen(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Poco::Timespan &duration) override;
	void unpairDevice(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device) override;
	void pingGateway(
			const ResultCall &resultCall,
			const Gateway &gateway) override;
	void updateActor(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device,
			const ModuleInfo &module,
			double value,
			const Timespan &timeout) override;

private:
	DeviceDao::Ptr m_deviceDao;
};

FakeGatewayRPC::FakeGatewayRPC()
{
}

void FakeGatewayRPC::setDeviceDao(DeviceDao::Ptr dao)
{
	m_deviceDao = dao;
}

void FakeGatewayRPC::sendListen(
		const ResultCall &resultCall,
		const Gateway &gateway,
		const Poco::Timespan &duration)
{
	logger().warning("send listen to gateway",
			__FILE__, __LINE__);

	GatewayRPCResult::Ptr result = new GatewayRPCResult;
	result->setStatus(GatewayRPCResult::SUCCESS);
	resultCall(result);
}

void FakeGatewayRPC::unpairDevice(
		const ResultCall &resultCall,
		const Gateway &gateway,
		const Device &target)
{
	Device device(target);

	GatewayRPCResult::Ptr result = new GatewayRPCResult;

	if (!m_deviceDao->fetch(device, gateway)) {
		logger().warning("failed to fetch device "
				+ device.id().toString(),
				__FILE__, __LINE__);

		result->setStatus(GatewayRPCResult::FAILED);
		resultCall(result);
		return;
	}

	device.setActiveSince(Nullable<DateTime>());

	if (!m_deviceDao->update(device, gateway)) {
		logger().warning("failed to update device "
				+ device.id().toString(),
				__FILE__, __LINE__);

		result->setStatus(GatewayRPCResult::FAILED);
		resultCall(result);
		return;
	}

	result->setStatus(GatewayRPCResult::SUCCESS);
	resultCall(result);
}

void FakeGatewayRPC::pingGateway(
		const ResultCall &resultCall,
		const Gateway &gateway)
{
	logger().warning("ping gateway",
			__FILE__, __LINE__);

	GatewayRPCResult::Ptr result = new GatewayRPCResult;
	result->setStatus(GatewayRPCResult::SUCCESS);
	resultCall(result);
}

void FakeGatewayRPC::updateActor(
		const ResultCall &resultCall,
		const Gateway &gateway,
		const Device &device,
		const ModuleInfo &module,
		double value,
		const Timespan &timeout)
{
	logger().warning("update actor",
			__FILE__, __LINE__);

	GatewayRPCResult::Ptr result = new GatewayRPCResult;
	result->setStatus(GatewayRPCResult::SUCCESS);
	resultCall(result);
}

}

BEEEON_OBJECT_BEGIN(BeeeOn, FakeGatewayRPC)
BEEEON_OBJECT_CASTABLE(GatewayRPC)
BEEEON_OBJECT_REF("deviceDao", &FakeGatewayRPC::setDeviceDao)
BEEEON_OBJECT_END(BeeeOn, FakeGatewayRPC)
