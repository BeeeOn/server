#ifndef BEEEON_GATEWAY_SCAN_WORK_EXECUTOR_H
#define BEEEON_GATEWAY_SCAN_WORK_EXECUTOR_H

#include "work/GatewayScanWork.h"
#include "util/Loggable.h"
#include "work/ExtendedWorkExecutor.h"

namespace BeeeOn {

class GatewayRPC;

/**
 * Requests the given gateway to start listening for new devices.
 * The work assumes that the listen takes the GatewayScanWork::duration()
 * time and it is finished after this timeout is reached.
 *
 * The operation itself can however succeed earlier because the GatewayRPCResult::SUCCESS
 * means that the command has been processed by the gateway.
 *
 * The transaction context is not required,
 */
class GatewayScanWorkExecutor :
		public ExtendedWorkExecutor,
		Loggable {
public:
	GatewayScanWorkExecutor();

	void setGatewayRPC(GatewayRPC *rpc);

	bool accepts(const Work::Ptr work) const;
	void execute(Work::Ptr work) override;

protected:
	void processResult(Work::Ptr work, GatewayScanWork &content);

private:
	GatewayRPC *m_rpc;
};

}

#endif
