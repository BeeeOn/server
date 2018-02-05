#pragma once

#include <Poco/DateTime.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/X509Certificate.h>

#include "gws/GatewayPeerVerifier.h"
#include "gws/SocketGatewayPeerVerifierFactory.h"
#include "model/Gateway.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief Implementation of the GatewayPeerVerifier interface using
 * the X509 certificates received via SSL/TLS layer.
 */
class X509GatewayPeerVerifier :
	public GatewayPeerVerifier,
	Loggable {
public:
	typedef Poco::SharedPtr<X509GatewayPeerVerifier> Ptr;

	X509GatewayPeerVerifier(const Poco::Net::X509Certificate &cert);

	/**
	 * @brief Verify that the given gateways'ID match the CN field of
	 * the underlying certificate.
	 *
	 * @throws Poco::Net::NotAuthenticatedException if the peer certificate is missing
	 * @throws Poco::Net::CertificateValidationException if the gateway ID and cert CN do not match
	 */
	void verifyPeer(const Gateway &gateway) const override;

	/**
	 * @return the underlying X509 certificate.
	 */
	const Poco::Net::X509Certificate &certificate() const;

	/**
	 * @returns Gateway ID extracted from the CN of the underlying X509 certificate.
	 */
	GatewayID extractID() const;

	static std::string formatDT(const Poco::DateTime &dt);
	static std::string formatCert(const Poco::Net::X509Certificate &cert);

private:
	const Poco::Net::X509Certificate m_peerCertificate;
};

class X509GatewayPeerVerifierFactory : public SocketGatewayPeerVerifierFactory, Loggable {
public:
	/**
	 * @returns X509GatewayPeerVerifier instance from for given socket that is
	 * expected to be a TLS/SSL socket.
	 */
	GatewayPeerVerifier::Ptr preverifyAndCreate(
			const Poco::Net::StreamSocket &socket) override;
};

}
