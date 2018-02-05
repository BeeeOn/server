#pragma once

#include <Poco/SharedPtr.h>

#include "model/Gateway.h"

namespace BeeeOn {

/**
 * @brief Interface that allows to verify gateway peer connection attempts.
 */
class GatewayPeerVerifier {
public:
	typedef Poco::SharedPtr<GatewayPeerVerifier> Ptr;

	virtual ~GatewayPeerVerifier();

	/**
	 * Verify that the underlying peer conforms to the given
	 * gateway. This usually means that the peer provides a
	 * proof that it is physically represents the given gateway.
	 *
	 * @throws Poco::Net::NotAuthenticatedException if the verification fails
	 */
	virtual void verifyPeer(const Gateway &gateway) const = 0;
};

class AcceptingGatewayPeerVerifier : public GatewayPeerVerifier {
public:
	void verifyPeer(const Gateway &gateway) const override;
};

}
