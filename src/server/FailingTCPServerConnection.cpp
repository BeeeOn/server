#include <Poco/Net/NetException.h>
#include <Poco/Net/SocketAddress.h>

#include "server/FailingTCPServerConnection.h"

using namespace Poco::Net;
using namespace BeeeOn;

FailingTCPServerConnection::FailingTCPServerConnection(
		const StreamSocket &socket):
	TCPServerConnection(socket)
{
}

void FailingTCPServerConnection::run()
{
	const SocketAddress &peer = socket().peerAddress();
	socket().close();
	throw ConnectionAbortedException(peer.toString());
}
