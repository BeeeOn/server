#include <Poco/Exception.h>
#include <Poco/Net/SecureServerSocket.h>

#include "di/Injectable.h"
#include "gws/WebSocketServer.h"
#include "gws/WebSocketRequestHandler.h"

BEEEON_OBJECT_BEGIN(BeeeOn, WebSocketServer)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_REF("gatewayCommunicator", &WebSocketServer::setGatewayCommunicator)
BEEEON_OBJECT_REF("sslConfig", &WebSocketServer::setSSLConfig)
BEEEON_OBJECT_REF("gatewayService", &WebSocketServer::setGatewayService)
BEEEON_OBJECT_NUMBER("port", &WebSocketServer::setPort)
BEEEON_OBJECT_NUMBER("backlog", &WebSocketServer::setBacklog)
BEEEON_OBJECT_NUMBER("maxMessageSize", &WebSocketServer::setMaxMessageSize)
BEEEON_OBJECT_NUMBER("minThreads", &WebSocketServer::setMinThreads)
BEEEON_OBJECT_NUMBER("maxThreads", &WebSocketServer::setMaxThreads)
BEEEON_OBJECT_TIME("threadIdleTime", &WebSocketServer::setThreadIdleTime)
BEEEON_OBJECT_END(BeeeOn, WebSocketServer)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

WebSocketServer::WebSocketServer():
	m_port(0),
	m_backlog(64),
	m_maxMessageSize(256)
{
}

void WebSocketServer::setGatewayCommunicator(GatewayCommunicator::Ptr communicator)
{
	m_gatewayCommunicator = communicator;
}

void WebSocketServer::setSSLConfig(SharedPtr<SSLServer> config)
{
	m_sslConfig = config;
}

void WebSocketServer::setGatewayService(GWSGatewayService::Ptr service)
{
	m_gatewayService = service;
}

void WebSocketServer::setPort(int port)
{
	if (port < 0)
		throw InvalidArgumentException("port must be non-negative");

	m_port = port;
}

void WebSocketServer::setBacklog(int backlog)
{
	if (backlog < 0)
		throw InvalidArgumentException("backlog must be non-negative");

	m_backlog = backlog;
}

void WebSocketServer::setMaxMessageSize(int size)
{
	if (size < 0)
		throw InvalidArgumentException("size must be non-negative");

	m_maxMessageSize = size;
}

Poco::Net::HTTPServer *WebSocketServer::createServer()
{
	HTTPRequestHandlerFactory::Ptr factory(
		new WebSocketRequestHandlerFactory(
			m_maxMessageSize,
			m_gatewayCommunicator,
			m_gatewayService
		));

	return new HTTPServer(factory, pool(), createSocket(), new HTTPServerParams);
}

ServerSocket WebSocketServer::createSocket()
{
	return m_sslConfig.isNull() ? ServerSocket(m_port, m_backlog)
			: SecureServerSocket(m_port, m_backlog, m_sslConfig->context());
}

void WebSocketServer::start()
{
	m_server = createServer();

	logger().information("starting WebSocket server on port: "
			+ to_string(m_server->port()), __FILE__, __LINE__);

	m_server->start();
}

void WebSocketServer::stop()
{
	logger().information("stopping WebSocket server", __FILE__, __LINE__);
	m_server->stop();
}
