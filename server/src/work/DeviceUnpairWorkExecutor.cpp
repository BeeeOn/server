#include <string>

#include <Poco/Exception.h>

#include "dao/DeviceDao.h"
#include "di/Injectable.h"
#include "rpc/GatewayRPC.h"
#include "work/DeviceUnpairWorkExecutor.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DeviceUnpairWorkExecutor)
BEEEON_OBJECT_CASTABLE(WorkExecutor)
BEEEON_OBJECT_REF("deviceDao", &DeviceUnpairWorkExecutor::setDeviceDao)
BEEEON_OBJECT_REF("gatewayRPC", &DeviceUnpairWorkExecutor::setGatewayRPC)
BEEEON_OBJECT_NUMBER("pollSleep", &DeviceUnpairWorkExecutor::setPollSleep)
BEEEON_OBJECT_NUMBER("maxAttempts", &DeviceUnpairWorkExecutor::setMaxAttempts)
BEEEON_OBJECT_END(BeeeOn, DeviceUnpairWorkExecutor)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DeviceUnpairWorkExecutor::DeviceUnpairWorkExecutor():
	m_dao(&NullDeviceDao::instance()),
	m_rpc(&NullGatewayRPC::instance()),
	m_pollSleep(800),
	m_maxAttempts(5)
{
}

void DeviceUnpairWorkExecutor::setDeviceDao(DeviceDao *dao)
{
	m_dao = dao == NULL? &NullDeviceDao::instance() : dao;
}

void DeviceUnpairWorkExecutor::setGatewayRPC(GatewayRPC *rpc)
{
	m_rpc = rpc == NULL? &NullGatewayRPC::instance() : rpc;
}

void DeviceUnpairWorkExecutor::setPollSleep(int ms)
{
	if (ms < 0)
		throw InvalidArgumentException("poll sleep must be non-negative");

	m_pollSleep = ms * 1000;
}

void DeviceUnpairWorkExecutor::setMaxAttempts(int count)
{
	if (count < 0)
		throw InvalidArgumentException("attempts count must be positive");

	m_maxAttempts = count;
}

bool DeviceUnpairWorkExecutor::accepts(const Work::Ptr work) const
{
	if (work.cast<DeviceUnpairWork>().isNull())
		return false;

	return true;
}

bool DeviceUnpairWorkExecutor::done(Device &device)
{
	const Gateway &gateway = device.gateway();

	if (!m_dao->fetch(device, gateway)) {
		if (logger().debug()) {
			logger().debug("device " + device
				+ " was not found in gateway " + gateway,
				__FILE__, __LINE__);
		}

		return true;
	}

	if (device.activeSince().isNull()) {
		if (logger().debug()) {
			logger().debug("device " + device
				+ " is inactive in gateway " + gateway,
				__FILE__, __LINE__);
		}

		return true;
	}

	return false;
}

void DeviceUnpairWorkExecutor::checkAttempts(DeviceUnpairWork::Ptr work)
{
	if (work->attempt() >= m_maxAttempts) {
		if (logger().debug()) {
			logger().debug(
				"failed to unpair device " + work->device()
				+ " after " + to_string(m_maxAttempts)
				+ " attempts",
				__FILE__, __LINE__);
		}

		throw TimeoutException("failed to unpair device " + work->device());
	}
}

void DeviceUnpairWorkExecutor::execute(Work::Ptr work)
{
	DeviceUnpairWork::Ptr unpair = work.cast<DeviceUnpairWork>();
	Device device(unpair->device());

	if (done(device))
		return;

	const Gateway &gateway = device.gateway();

	checkAttempts(unpair);
	unpair->setAttempt(unpair->attempt() + 1);

	try {
		m_rpc->unpairDevice(gateway, device);
	} catch (const Poco::Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		// we will try again after suspend
	}

	Timespan sleep(m_pollSleep * 1000);
	if (device.refresh() >= Timespan(1, 0))
		sleep = device.refresh();

	suspend(work, sleep);
}
