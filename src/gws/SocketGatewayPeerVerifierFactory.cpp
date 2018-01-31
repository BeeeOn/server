#include <Poco/Exception.h>

#include "gws/SocketGatewayPeerVerifierFactory.h"

using namespace BeeeOn;
using namespace Poco;
using namespace Poco::Net;

SocketGatewayPeerVerifierFactory::~SocketGatewayPeerVerifierFactory()
{
}

const SecureStreamSocket &SocketGatewayPeerVerifierFactory::asSecure(
		const StreamSocket &socket)
{
	try {
		return dynamic_cast<const SecureStreamSocket &>(socket);
	}
	catch (...) {
		throw BadCastException("socket is not inherited from SecureStreamSocket");
	}
}

SharedPtr<X509Certificate> SocketGatewayPeerVerifierFactory::extractPeerCert(
		const StreamSocket &socket)
{
	SharedPtr<X509Certificate> certificate;

	try {
		const SecureStreamSocket &secureSocket = asSecure(socket);

		if (!secureSocket.havePeerCertificate())
			return certificate;

		certificate = new X509Certificate(secureSocket.peerCertificate());
	}
	catch (const BadCastException &e) {
	}

	return certificate;
}
