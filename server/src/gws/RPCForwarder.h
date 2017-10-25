#ifndef BEEEON_RPC_FORWARDER_H
#define BEEEON_RPC_FORWARDER_H

#include <Poco/SharedPtr.h>

#include "gwmessage/GWResponse.h"
#include "model/GatewayID.h"

namespace BeeeOn {

/**
 * @brief RPCForwarder provides an interface for the Gateway Server
 * how to forward responses from the gateways to the UI Server.
 */
class RPCForwarder {
public:
	typedef Poco::SharedPtr<RPCForwarder> Ptr;

	virtual ~RPCForwarder();

	/**
	 * @brief Forwards response to the UI Server.
	 */
	virtual void forwardResponse(const GatewayID &gatewayID,
		GWResponse::Ptr response) = 0;

	/**
	 * @brief Forwards timeout response to the UI Server.
	 */
	virtual void forwardTimeout(const GatewayID &gatewayID,
		const GlobalID &responseID) = 0;
};

}

#endif
