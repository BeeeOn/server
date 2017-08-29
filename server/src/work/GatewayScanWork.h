#ifndef BEEEON_GATEWAY_SCAN_WORK_H
#define BEEEON_GATEWAY_SCAN_WORK_H

#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>

#include "model/Gateway.h"
#include "rpc/GatewayRPCResult.h"
#include "work/AbstractGatewayWork.h"

namespace BeeeOn {

/**
 * Represents an asynchronous gateway listen operation context.
 */
class GatewayScanWork : public AbstractGatewayWork {
public:
	GatewayScanWork();
	GatewayScanWork(const WorkContent &content);
	GatewayScanWork(const Poco::JSON::Object::Ptr json);

	void setGatewayID(const GatewayID &id);
	GatewayID gatewayID() const;

	void setDuration(const Poco::Timespan &duration);
	Poco::Timespan duration() const;

	void setStarted(const Poco::Timestamp &time);
	Poco::Timestamp started() const;
};

}

#endif
