#include <string>

#include <Poco/Exception.h>
#include <Poco/Net/SecureServerSocket.h>

#include "ssl/SSLServer.h"
#include "server/SocketServer.h"
#include "Debug.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

SocketServer::SocketServer():
	m_port(0),
	m_backlog(64),
	m_sslConfig(NULL)
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
	server->setSSLConfig(sslConfig);
	server->setPort(port);

	return server;
}

void SocketServer::setSSLConfig(SSLServer *config)
{
	m_sslConfig = config;
}

void SocketServer::setPort(int port)
{
	if (port < 0) {
		throw InvalidArgumentException(
				"invalid port number: " + to_string(port));
	}

	m_port = (unsigned int) port;
}

void SocketServer::setBacklog(int backlog)
{
	if (backlog < 1) {
		throw InvalidArgumentException(
				"invalid backlog: " + to_string(backlog));
	}

	m_backlog = backlog;
}

void SocketServer::setFactory(TCPServerConnectionFactory::Ptr factory)
{
	m_factory = factory;
}

void SocketServer::setTCPParams(const TCPServerParams::Ptr params)
{
	m_tcpParams = params;
}

TCPServer *SocketServer::createServer()
{
	if (m_sslConfig == NULL) {
		ServerSocket socket(m_port, m_backlog);
		return new TCPServer(m_factory, socket, m_tcpParams);
	}

	Context::Ptr context = m_sslConfig->context();
	SecureServerSocket socket(m_port, m_backlog, context);
	return new TCPServer(m_factory, socket, m_tcpParams);
}

void SocketServer::start()
{
	m_server = createServer();

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
