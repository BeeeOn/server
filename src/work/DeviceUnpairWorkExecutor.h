#ifndef BEEEON_DEVICE_UNPAIR_WORK_EXECUTOR_H
#define BEEEON_DEVICE_UNPAIR_WORK_EXECUTOR_H

#include "dao/DeviceDao.h"
#include "work/DeviceUnpairWork.h"
#include "util/Loggable.h"
#include "rpc/GatewayRPC.h"
#include "work/ExtendedWorkExecutor.h"

namespace BeeeOn {

/**
 * Executes DeviceUnpairWork instances.
 */
class DeviceUnpairWorkExecutor :
		public ExtendedWorkExecutor,
		Loggable {
public:
	DeviceUnpairWorkExecutor();

	void setDeviceDao(DeviceDao::Ptr dao);
	void setGatewayRPC(GatewayRPC::Ptr rpc);

	bool accepts(const Work::Ptr work) const override;
	void execute(Work::Ptr work) override;

protected:
	/**
	 * Process the result of the work. It is assumed that the DeviceUnpairWork
	 * contains a result of its operation.
	 *
	 * @see DeviceUnpairWork::hasResult()
	 */
	void processResult(Work::Ptr work, DeviceUnpairWork &content);

private:
	DeviceDao::Ptr m_dao;
	GatewayRPC::Ptr m_rpc;
};

}

#endif
