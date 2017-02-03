#include <Poco/Net/SecureServerSocket.h>

#include "server/SocketServer.h"
#include "Debug.h"

using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

SocketServer::SocketServer(
		TCPServerConnectionFactory::Ptr factory,
		const ServerSocket &socket,
		TCPServerParams::Ptr params):
	m_factory(factory),
	m_socket(socket),
	m_tcpParams(params)
{
}

SocketServer *SocketServer::createDefault(
	TCPServerConnectionFactory::Ptr factory,
	UInt16 port)
{
	ServerSocket socket(port);
	TCPServerParams::Ptr params = new TCPServerParams();
	return new SocketServer(factory, socket, params);
}

SocketServer *SocketServer::createSecure(
	TCPServerConnectionFactory::Ptr factory,
	Context::Ptr context,
	UInt16 port)
{
	SecureServerSocket socket(port, 64, context);
	TCPServerParams::Ptr params = new TCPServerParams();
	return new SocketServer(factory, socket, params);
}

void SocketServer::start()
{
	m_server = new TCPServer(m_factory, m_socket, m_tcpParams);

	LOGGER_CLASS(this).information(
		"starting server on port "
		+ std::to_string(m_server->port()));

	m_server->start();
}

void SocketServer::stop()
{
	LOGGER_CLASS(this).information(
		"stopping server on port "
		+ std::to_string(m_server->port()));

	m_server->stop();
}
