#ifndef BEEEON_DEVICE_UNPAIR_WORK_EXECUTOR_H
#define BEEEON_DEVICE_UNPAIR_WORK_EXECUTOR_H

#include "work/DeviceUnpairWork.h"
#include "util/Loggable.h"
#include "work/WorkExecutor.h"

namespace BeeeOn {

class DeviceDao;
class GatewayRPC;

/**
 * Executes DeviceUnpairWork instances. It should be executed
 * in a transaction context to avoid concurrency issues.
 */
class DeviceUnpairWorkExecutor :
		public WorkExecutor,
		Loggable {
public:
	DeviceUnpairWorkExecutor();

	void setDeviceDao(DeviceDao *dao);
	void setGatewayRPC(GatewayRPC *rpc);

	/**
	 * How long (in miliseconds) to sleep before database polling steps.
	 * This sleep time is applicated when a device has no or unknown
	 * refresh time.
	 */
	void setPollSleep(int ms);

	/**
	 * How many attempts to perform before we can consider
	 * an executed work as failing. This corresponds to the
	 * DeviceUnpairWork::attempts property.
	 */
	void setMaxAttempts(int count);

	bool accepts(const Work::Ptr work) const;
	void execute(Work::Ptr work) override;

protected:
	/**
	 * Test whether a task targeting the given device has finished
	 * successfully.
	 */
	bool done(Device &device);

	/**
	 * Test whether we have exceeded the attempts count. In that case,
	 * the Poco::TimeoutException is thrown and the task is marked as
	 * failing.
	 */
	void checkAttempts(DeviceUnpairWork::Ptr work);

private:
	DeviceDao *m_dao;
	GatewayRPC *m_rpc;
	unsigned long m_pollSleep;
	unsigned int m_maxAttempts;
};

}

#endif
