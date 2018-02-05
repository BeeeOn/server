#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Net/X509Certificate.h>

#include "gws/SocketGatewayPeerVerifierFactory.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief WRapper around any GatewayPeerVerifier implementation. It relaxes
 * verification process of the underlying verifier to just logging its behaviour.
 * It accepts all gateway peers. The implementation is intended for testing.
 */
class RelaxedGatewayPeerVerifier : public GatewayPeerVerifier, Loggable {
public:
	RelaxedGatewayPeerVerifier(GatewayPeerVerifier::Ptr verifier);

	/**
	 * Use another GatewayPeerVerifier instance to verify the gateway but
	 * never fail (accept all peers).
	 */
	void verifyPeer(const Gateway &gateway) const override;

private:
	GatewayPeerVerifier::Ptr m_verifier;
};

/**
 * @brief The SocketGatewayPeerVerifierFactory implementation designed to wrap another
 * SocketGatewayPeerVerifierFactory. If the underlying preverifyAndCreate() call fails,
 * it using the AcceptingGatewayPeerVerifier instance instead to accept all gateway peers.
 */
class RelaxedGatewayPeerVerifierFactory : public SocketGatewayPeerVerifierFactory, Loggable {
public:
	RelaxedGatewayPeerVerifierFactory();

	GatewayPeerVerifier::Ptr preverifyAndCreate(
			const Poco::Net::StreamSocket &socket) override;

	void setVerifierFactory(SocketGatewayPeerVerifierFactory::Ptr factory);

private:
	SocketGatewayPeerVerifierFactory::Ptr m_factory;
	GatewayPeerVerifier::Ptr m_acceptingVerifier;
};

}
