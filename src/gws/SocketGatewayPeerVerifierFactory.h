#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/X509Certificate.h>

#include "gws/GatewayPeerVerifier.h"

namespace BeeeOn {

/**
 * @brief Factory to create GatewayPeerVerifier instances based on StreamSockets.
 */
class SocketGatewayPeerVerifierFactory {
public:
	typedef Poco::SharedPtr<SocketGatewayPeerVerifierFactory> Ptr;

	virtual ~SocketGatewayPeerVerifierFactory();

	/**
	 * Pre-verifies the given socket and creates an instance
	 * of the GatewayPeerVerifier. The pre-verification can
	 * e.g. test whether the given connection is secure or
	 * whether it provides a peer certificate.
	 *
	 * @throws Poco::Net::NotAuthenticatedException if the pre-verification fails
	 */
	virtual GatewayPeerVerifier::Ptr preverifyAndCreate(
			const Poco::Net::StreamSocket &socket) = 0;

protected:
	/**
	 * Convert the given instance of StreamSocket into the SecureStreamSocket
	 * if possible.
	 *
	 * @throws Poco::BadCastException if the StreamSocket cannot be casted
	 */
	static const Poco::Net::SecureStreamSocket &asSecure(
			const Poco::Net::StreamSocket &socket);

	/**
	 * Extract the peer certificate from the given StreamSocket instance
	 * if possible. It returns null result when the socket is not secure
	 * or there is no peer certificate available.
	 */
	static Poco::SharedPtr<Poco::Net::X509Certificate> extractPeerCert(
			const Poco::Net::StreamSocket &socket);
};

}
