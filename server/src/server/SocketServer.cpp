#include <Poco/Net/SecureServerSocket.h>

#include "ssl/SSLServer.h"
#include "server/SocketServer.h"
#include "Debug.h"

using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

SocketServer::SocketServer()
{
}

SocketServer *SocketServer::create(
	TCPServerConnectionFactory::Ptr factory,
	SSLServer *sslConfig,
	UInt16 port)
{
	SocketServer *server = new SocketServer();

	server->setFactory(factory);
	server->setTCPParams(new TCPServerParams());

	if (sslConfig == NULL)
		server->setSocket(ServerSocket(port));
	else
		server->setSocket(SecureServerSocket(port, 64, sslConfig->context()));

	return server;
}

void SocketServer::setFactory(TCPServerConnectionFactory::Ptr factory)
{
	m_factory = factory;
}

void SocketServer::setSocket(const ServerSocket &socket)
{
	m_socket = socket;
}

void SocketServer::setTCPParams(const TCPServerParams::Ptr params)
{
	m_tcpParams = params;
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
