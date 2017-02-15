#include <Poco/Net/SocketStream.h>
#include <Poco/Net/StreamSocket.h>

#include "di/Injectable.h"
#include "rpc/SocketRPCConnector.h"

using namespace std;
using namespace Poco::Net;
using namespace BeeeOn;

SocketRPCConnector::SocketRPCConnector():
	m_port(0)
{
}

void SocketRPCConnector::open()
{
	m_socket.assign(new StreamSocket(SocketAddress(m_host, m_port)));
	m_socket->setReceiveTimeout(m_receiveTimeout);

	m_socketStream.assign(new SocketStream(*m_socket));
}

ostream &SocketRPCConnector::send()
{
	return *m_socketStream;
}

void SocketRPCConnector::flush()
{
	m_socketStream->flush();
}

istream &SocketRPCConnector::receive()
{
	return *m_socketStream;
}

void SocketRPCConnector::close()
{
	//Flushes the stream and closes the socket.
	m_socketStream->close();
}

BEEEON_OBJECT_BEGIN(BeeeOn, SocketRPCConnector)
BEEEON_OBJECT_CASTABLE(RPCConnector)
BEEEON_OBJECT_TEXT("host", &SocketRPCConnector::setHost)
BEEEON_OBJECT_NUMBER("port", &SocketRPCConnector::setPort)
BEEEON_OBJECT_NUMBER("receiveTimeout", &SocketRPCConnector::setReceiveTimeout)
BEEEON_OBJECT_END(BeeeOn, SocketRPCConnector)