#include <Poco/Net/NetException.h>

#include "gws/GatewayPeerVerifier.h"

using namespace BeeeOn;
using namespace Poco::Net;

GatewayPeerVerifier::~GatewayPeerVerifier()
{
}

AbstractGatewayPeerVerifier::AbstractGatewayPeerVerifier(const GatewayID &id):
	m_expectedID(id)
{
}

void AbstractGatewayPeerVerifier::verifyPeer(const Gateway &gateway) const
{
	if (gateway.id() == m_expectedID)
		return;

	throw NotAuthenticatedException(
		"gateway ID mismatch, got "
		+ gateway.id().toString()
		+ " but expected "
		+ gateway.id().toString());
}
