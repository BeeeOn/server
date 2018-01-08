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

	/**
	 * Called when the remote command state changes however
	 * it is still pending. We do not know whether it has been
	 * delivered to the gateway.
	 */
	virtual void onPending(GatewayRPCResult::Ptr result);

	/**
	 * Called when the remote command has been accepted by
	 * the gateway and it is going to be executed.
	 */
	virtual void onAccepted(GatewayRPCResult::Ptr result);

	/**
	 * Called when the remote command has been successfully
	 * executed by the gateway and we get the results here.
	 */
	virtual void onSuccess(GatewayRPCResult::Ptr result);

	/**
	 * Called when the remote command cannot be performed
	 * because the gateway is not connected.
	 */
	virtual void onNotConnected(GatewayRPCResult::Ptr result);

	/**
	 * Called when the timeout of the remote command exceeds
	 * when trying to contact the gateway.
	 */
	virtual void onTimeout(GatewayRPCResult::Ptr result);

	/**
	 * Called when the gateway has failed to perform the remote
	 * command for some reason.
	 */
	virtual void onFailed(GatewayRPCResult::Ptr result);

protected:
	/**
	 * Handle the given result and invoke some of the event handlers.
	 */
	void handle(GatewayRPCResult::Ptr result);
};

}
