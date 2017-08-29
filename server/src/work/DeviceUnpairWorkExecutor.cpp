#include <string>

#include <Poco/Event.h>
#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>

#include "dao/DeviceDao.h"
#include "di/Injectable.h"
#include "rpc/GatewayRPC.h"
#include "work/DeviceUnpairWorkExecutor.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DeviceUnpairWorkExecutor)
BEEEON_OBJECT_CASTABLE(WorkExecutor)
BEEEON_OBJECT_REF("deviceDao", &DeviceUnpairWorkExecutor::setDeviceDao)
BEEEON_OBJECT_REF("gatewayRPC", &DeviceUnpairWorkExecutor::setGatewayRPC)
BEEEON_OBJECT_REF("scheduler", &DeviceUnpairWorkExecutor::setScheduler)
BEEEON_OBJECT_END(BeeeOn, DeviceUnpairWorkExecutor)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

DeviceUnpairWorkExecutor::DeviceUnpairWorkExecutor():
	m_rpc(&NullGatewayRPC::instance())
{
}

void DeviceUnpairWorkExecutor::setDeviceDao(DeviceDao::Ptr dao)
{
	m_dao = dao;
}

void DeviceUnpairWorkExecutor::setGatewayRPC(GatewayRPC *rpc)
{
	m_rpc = rpc == NULL? &NullGatewayRPC::instance() : rpc;
}

bool DeviceUnpairWorkExecutor::accepts(const Work::Ptr work) const
{
	if (work->content().type().is<DeviceUnpairWork>())
		return true;

	return false;
}

void DeviceUnpairWorkExecutor::processResult(Work::Ptr work, DeviceUnpairWork &content)
{
	Gateway gateway(content.gatewayID());
	Device device(content.deviceID());

	switch (content.result()) {
	case GatewayRPCResult::NOT_CONNECTED:
		throw ConnectionAbortedException(
			"failed to unpair device " + device + " on gateway " + gateway);

	case GatewayRPCResult::TIMEOUT:
		throw TimeoutException(
			"failed to unpair device " + device + " on gateway " + gateway);

	case GatewayRPCResult::FAILED:
		throw RuntimeException(
			"failed to unpair device " + device + " on gateway " + gateway);

	case GatewayRPCResult::SUCCESS:
		return;

	default:
		suspend(work);
		break;
	}
}

void DeviceUnpairWorkExecutor::execute(Work::Ptr work)
{
	DeviceUnpairWork content = work->contentAs<DeviceUnpairWork>();

	Device device(content.deviceID());
	const Gateway gateway(content.gatewayID());

	if (!m_dao->fetch(device, gateway)) {
		logger().warning("device "
				+ device
				+ " on gateway "
				+ gateway
				+ " not found during unpair",
				__FILE__, __LINE__);
		return; // treat as success
	}

	if (content.hasResult()) {
		processResult(work, content);
		return;
	}

	WorkScheduler::Ptr scheduler = this->scheduler();
	WorkLockManager::Ptr lockManager = this->lockManager();

	m_rpc->unpairDevice([=](GatewayRPCResult::Ptr result) mutable
		{
			{
				WorkWriteGuard accessGuard(lockManager->readWrite(work->id()));

				// update result of the work
				DeviceUnpairWork content = work->contentAs<DeviceUnpairWork>();
				content.setResult(result->status());
				work->setContent(content);
			}

			switch (result->status()) {
			case GatewayRPCResult::PENDING:
			case GatewayRPCResult::ACCEPTED:
				// TODO persist
				return; // no other action is needed
			default:
				scheduler->wakeup(work);
				break;
			}
		},
		gateway,
		device
	);

	suspend(work);
}
