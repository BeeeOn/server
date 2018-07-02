#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "rpc/GatewayRPC.h"
#include "dao/DeviceDao.h"
#include "util/AsyncExecutor.h"
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
	void setAsyncExecutor(AsyncExecutor::Ptr executor);

	void sendListen(
			GatewayRPCHandler::Ptr handler,
			const Gateway &gateway,
			const Poco::Timespan &duration) override;
	void pairDevice(
			GatewayRPCHandler::Ptr handler,
			const Gateway &gateway,
			const Device &device) override;
	void unpairDevice(
			GatewayRPCHandler::Ptr handler,
			const Gateway &gateway,
			const Device &device) override;
	void updateActor(
			GatewayRPCHandler::Ptr handler,
			const Gateway &gateway,
			const Device &device,
			const ModuleInfo &module,
			double value,
			const Timespan &timeout) override;

private:
	DeviceDao::Ptr m_deviceDao;
	AsyncExecutor::Ptr m_executor;
};

FakeGatewayRPC::FakeGatewayRPC()
{
}

void FakeGatewayRPC::setDeviceDao(DeviceDao::Ptr dao)
{
	m_deviceDao = dao;
}

void FakeGatewayRPC::setAsyncExecutor(Poco::SharedPtr<AsyncExecutor> executor)
{
	m_executor = executor;
}

void FakeGatewayRPC::sendListen(
		GatewayRPCHandler::Ptr handler,
		const Gateway &gateway,
		const Poco::Timespan &duration)
{
	logger().warning("send listen to gateway",
			__FILE__, __LINE__);

	GatewayRPCResult::Ptr result = new GatewayRPCResult;
	result->setStatus(GatewayRPCResult::Status::SUCCESS);

	m_executor->invoke([handler, result]()
	{
		doHandle(handler, result);
	});
}

void FakeGatewayRPC::pairDevice(
		GatewayRPCHandler::Ptr handler,
		const Gateway &gateway,
		const Device &target)
{
	Device device(target);

	GatewayRPCResult::Ptr result = new GatewayRPCResult;

	if (!m_deviceDao->fetch(device, gateway)) {
		logger().warning("failed to fetch device "
				+ device.id().toString(),
				__FILE__, __LINE__);

		result->setStatus(GatewayRPCResult::Status::FAILED);

		m_executor->invoke([handler, result]()
		{
			doHandle(handler, result);
		});

		return;
	}

	result->setStatus(GatewayRPCResult::Status::SUCCESS);

	m_executor->invoke([handler, result]()
	{
		doHandle(handler, result);
	});
}


void FakeGatewayRPC::unpairDevice(
		GatewayRPCHandler::Ptr handler,
		const Gateway &gateway,
		const Device &target)
{
	Device device(target);

	GatewayRPCResult::Ptr result = new GatewayRPCResult;

	if (!m_deviceDao->fetch(device, gateway)) {
		logger().warning("failed to fetch device "
				+ device.id().toString(),
				__FILE__, __LINE__);

		result->setStatus(GatewayRPCResult::Status::FAILED);

		m_executor->invoke([handler, result]()
		{
			doHandle(handler, result);
		});

		return;
	}

	device.status().setState(DeviceStatus::STATE_INACTIVE);
	device.status().setLastChanged({});

	if (!m_deviceDao->update(device, gateway)) {
		logger().warning("failed to update device "
				+ device.id().toString(),
				__FILE__, __LINE__);

		result->setStatus(GatewayRPCResult::Status::FAILED);

		m_executor->invoke([handler, result]()
		{
			doHandle(handler, result);
		});

		return;
	}

	result->setStatus(GatewayRPCResult::Status::SUCCESS);

	m_executor->invoke([handler, result]()
	{
		doHandle(handler, result);
	});
}

void FakeGatewayRPC::updateActor(
		GatewayRPCHandler::Ptr handler,
		const Gateway &gateway,
		const Device &device,
		const ModuleInfo &module,
		double value,
		const Timespan &timeout)
{
	logger().warning("update actor",
			__FILE__, __LINE__);

	GatewayRPCResult::Ptr result = new GatewayRPCResult;
	result->setStatus(GatewayRPCResult::Status::SUCCESS);

	m_executor->invoke([handler, result]()
	{
		doHandle(handler, result);
	});
}

}

BEEEON_OBJECT_BEGIN(BeeeOn, FakeGatewayRPC)
BEEEON_OBJECT_CASTABLE(GatewayRPC)
BEEEON_OBJECT_PROPERTY("deviceDao", &FakeGatewayRPC::setDeviceDao)
BEEEON_OBJECT_PROPERTY("asyncExecutor", &FakeGatewayRPC::setAsyncExecutor)
BEEEON_OBJECT_END(BeeeOn, FakeGatewayRPC)
