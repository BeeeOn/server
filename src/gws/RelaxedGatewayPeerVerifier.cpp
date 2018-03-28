#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "gws/RelaxedGatewayPeerVerifier.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RelaxedGatewayPeerVerifierFactory)
BEEEON_OBJECT_CASTABLE(SocketGatewayPeerVerifierFactory)
BEEEON_OBJECT_PROPERTY("verifierFactory", &RelaxedGatewayPeerVerifierFactory::setVerifierFactory)
BEEEON_OBJECT_END(BeeeOn, RelaxedGatewayPeerVerifierFactory)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

RelaxedGatewayPeerVerifier::RelaxedGatewayPeerVerifier(
		GatewayPeerVerifier::Ptr verifier):
	m_verifier(verifier)
{
}

void RelaxedGatewayPeerVerifier::verifyPeer(const Gateway &gateway) const
{
	try {
		m_verifier->verifyPeer(gateway);

		logger().notice(
			"gateway " + gateway.id().toString()
			+ " was accepted in relaxed mode",
			__FILE__, __LINE__);
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
	}
}

RelaxedGatewayPeerVerifierFactory::RelaxedGatewayPeerVerifierFactory():
	m_acceptingVerifier(new AcceptingGatewayPeerVerifier)
{
	logger().critical("ALL GATEWAYS ARE ACCEPTED REGARDLESS OF THE VERIFICATION PROCESS",
			__FILE__, __LINE__);
}

void RelaxedGatewayPeerVerifierFactory::setVerifierFactory(
		SocketGatewayPeerVerifierFactory::Ptr factory)
{
	m_factory = factory;
}

GatewayPeerVerifier::Ptr RelaxedGatewayPeerVerifierFactory::preverifyAndCreate(
		const StreamSocket &socket)
{
	try {
		return m_factory->preverifyAndCreate(socket);
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
	}

	return new RelaxedGatewayPeerVerifier(m_acceptingVerifier);
}
