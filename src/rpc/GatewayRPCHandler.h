#pragma once

#include <Poco/AutoPtr.h>
#include <Poco/RefCountedObject.h>

#include "rpc/GatewayRPCResult.h"

namespace BeeeOn {

/**
 * @brief Interface to deal with asynchronous communication with
 * a selected gateway.
 */
class GatewayRPCHandler : public Poco::RefCountedObject {
public:
	typedef Poco::AutoPtr<GatewayRPCHandler> Ptr;

	/**
	 * Called when a result update is delivered.
	 */
	virtual void onAny(GatewayRPCResult::Ptr result);
};

}
