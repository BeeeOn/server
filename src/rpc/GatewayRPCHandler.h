#pragma once

#include <functional>

#include <Poco/AutoPtr.h>
#include <Poco/RefCountedObject.h>

#include "rpc/GatewayRPCResult.h"

namespace BeeeOn {

/**
 * @brief Interface to deal with asynchronous communication with
 * a selected gateway.
 */
class GatewayRPCHandler : public Poco::RefCountedObject {
	friend class GatewayRPC;
public:
	typedef Poco::AutoPtr<GatewayRPCHandler> Ptr;

	/**
	 * Called when a result update is delivered.
	 */
	virtual void onAny(GatewayRPCResult::Ptr result);

protected:
	/**
	 * Handle the given result and invoke some of the event handlers.
	 */
	void handle(GatewayRPCResult::Ptr result);
};

}
