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
public:
	typedef Poco::AutoPtr<GatewayRPCHandler> Ptr;

	/**
	 * Construct lambda function calling this GatewayRPCHandler instance.
	 */
	std::function<void(GatewayRPCResult::Ptr)> lambda();

	/**
	 * Called when a result update is delivered.
	 */
	virtual void onAny(GatewayRPCResult::Ptr result);
};

}
