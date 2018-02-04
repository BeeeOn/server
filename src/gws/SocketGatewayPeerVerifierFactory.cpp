#include <Poco/Exception.h>
#include <Poco/Net/SecureStreamSocketImpl.h>

#include "gws/SocketGatewayPeerVerifierFactory.h"

using namespace BeeeOn;
using namespace Poco;
using namespace Poco::Net;

SocketGatewayPeerVerifierFactory::~SocketGatewayPeerVerifierFactory()
{
}

SharedPtr<X509Certificate> SocketGatewayPeerVerifierFactory::extractPeerCert(
		const StreamSocket &socket)
{
	SharedPtr<X509Certificate> certificate;

	SecureStreamSocketImpl *impl =
		dynamic_cast<SecureStreamSocketImpl *>(socket.impl());

	if (impl && impl->havePeerCertificate())
		certificate = new X509Certificate(impl->peerCertificate());

	return certificate;
}
