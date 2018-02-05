#include <Poco/Net/NetException.h>

#include "gws/GatewayPeerVerifier.h"

using namespace BeeeOn;
using namespace Poco::Net;

GatewayPeerVerifier::~GatewayPeerVerifier()
{
}

void AcceptingGatewayPeerVerifier::verifyPeer(const Gateway &gateway) const
{
}
