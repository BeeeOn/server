#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Net/X509Certificate.h>

#include "gws/GatewayPeerVerifier.h"
#include "gws/SocketGatewayPeerVerifierFactory.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief The GatewayPeerVerifier implementation that never fails and thus accepts
 * all peers with or without a ceritifate. The implementation is intended for
 * testing.
 */
class RelaxedGatewayPeerVerifier : public GatewayPeerVerifier, Loggable {
public:
	RelaxedGatewayPeerVerifier(Poco::SharedPtr<Poco::Net::X509Certificate> peerCertificate);

	/**
	 * Try to verify the peer certificate against the given gateway and report
	 * details and success of the operation. The implementation does not throw
	 * exceptions in case of a failure (accepts all peers).
	 */
	void verifyPeer(const Gateway &gateway) const override;

private:
	Poco::SharedPtr<Poco::Net::X509Certificate> m_peerCertificate;
};

/**
 * @brief The SocketGatewayPeerVerifierFactory implementation designed to accept all
 * secured or insecured peers. The pre-verification just reports the details and success
 * of the operation.
 */
class RelaxedGatewayPeerVerifierFactory : public SocketGatewayPeerVerifierFactory, Loggable {
public:
	GatewayPeerVerifier::Ptr preverifyAndCreate(
			const Poco::Net::StreamSocket &socket) override;
protected:
	Poco::SharedPtr<Poco::Net::X509Certificate> preverify(
			const Poco::Net::StreamSocket &socket) const;
};

}
