#include <string>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Timespan.h>
#include <Poco/Net/SecureServerSocket.h>

#include "di/Injectable.h"
#include "ssl/SSLServer.h"
#include "server/SocketServer.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, SocketServer)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("port", &SocketServer::setPort)
BEEEON_OBJECT_PROPERTY("backlog", &SocketServer::setBacklog)
BEEEON_OBJECT_PROPERTY("maxThreads", &SocketServer::setMaxThreads)
BEEEON_OBJECT_PROPERTY("maxQueued", &SocketServer::setMaxQueued)
BEEEON_OBJECT_PROPERTY("threadIdleTime", &SocketServer::setThreadIdleTime)
BEEEON_OBJECT_PROPERTY("threadPriority", &SocketServer::setThreadPriority)
BEEEON_OBJECT_PROPERTY("sslConfig", &SocketServer::setSSLConfig)
BEEEON_OBJECT_PROPERTY("connectionFactory", &SocketServer::setFactory)
BEEEON_OBJECT_END(BeeeOn, SocketServer)

SocketServer::SocketServer():
	m_port(0),
	m_backlog(64),
	m_sslConfig(NULL),
	m_tcpParams(new TCPServerParams())
{
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

void SocketServer::setFactory(SocketServerConnectionFactory::Ptr factory)
{
	m_factory = factory;
}

void SocketServer::setMaxThreads(int count)
{
	m_tcpParams->setMaxThreads(count);
}

void SocketServer::setMaxQueued(int count)
{
	m_tcpParams->setMaxQueued(count);
}

void SocketServer::setThreadIdleTime(const Timespan &time)
{
	m_tcpParams->setThreadIdleTime(time);
}

void SocketServer::setThreadPriority(const std::string &priority)
{
	Thread::Priority prio;

	if (priority == "lowest")
		prio = Thread::PRIO_LOWEST;
	else if (priority == "low")
		prio = Thread::PRIO_LOW;
	else if (priority == "normal")
		prio = Thread::PRIO_NORMAL;
	else if (priority == "high")
		prio = Thread::PRIO_HIGH;
	else if (priority == "highest")
		prio = Thread::PRIO_HIGHEST;
	else
		throw InvalidArgumentException("unrecognized priority: "
				+ priority);

	m_tcpParams->setThreadPriority(prio);
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

	logger().information(
		"starting server on port "
		+ std::to_string(m_server->port()));

	m_server->start();
}

void SocketServer::stop()
{
	logger().information(
		"stopping server on port "
		+ std::to_string(m_server->port()));

	m_server->stop();
}
