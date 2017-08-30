#ifndef BEEEON_ABSTRACT_GATEWAY_WORK_H
#define BEEEON_ABSTRACT_GATEWAY_WORK_H

#include "rpc/GatewayRPCResult.h"
#include "work/WorkContent.h"

namespace BeeeOn {

/**
 * AbstractGatewayWork is intended for works that controls some gateway
 * features remotely. E.g. it is possible to unpair a device. The unpairing
 * process is asynchronous and a progress can be reported multiple times.
 * Thus, all such works hold the last state of the remote operation.
 */
class AbstractGatewayWork : public WorkContent {
public:
	AbstractGatewayWork(const WorkContent &copy);
	AbstractGatewayWork(const ClassInfo &type);
	AbstractGatewayWork(const Poco::JSON::Object::Ptr content);

	void setResult(const GatewayRPCResult::Status status);
	GatewayRPCResult::Status result() const;

	/**
	 * If no result is set, the work has not been executed yet or
	 * there was no result received.
	 *
	 * @return true if a result has been defined
	 */
	bool hasResult() const;
};

}

#endif
