#include <Poco/Exception.h>
#include <Poco/Net/SecureServerSocket.h>

#include "di/Injectable.h"
#include "gws/WebSocketServer.h"
#include "gws/WebSocketRequestHandler.h"

BEEEON_OBJECT_BEGIN(BeeeOn, WebSocketServer)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("gatewayCommunicator", &WebSocketServer::setGatewayCommunicator)
BEEEON_OBJECT_PROPERTY("sslConfig", &WebSocketServer::setSSLConfig)
BEEEON_OBJECT_PROPERTY("gatewayService", &WebSocketServer::setGatewayService)
BEEEON_OBJECT_PROPERTY("verifierFactory", &WebSocketServer::setVerifierFactory)
BEEEON_OBJECT_PROPERTY("name", &WebSocketServer::setName)
BEEEON_OBJECT_PROPERTY("host", &WebSocketServer::setHost)
BEEEON_OBJECT_PROPERTY("port", &WebSocketServer::setPort)
BEEEON_OBJECT_PROPERTY("backlog", &WebSocketServer::setBacklog)
BEEEON_OBJECT_PROPERTY("maxMessageSize", &WebSocketServer::setMaxMessageSize)
BEEEON_OBJECT_PROPERTY("minThreads", &WebSocketServer::setMinThreads)
BEEEON_OBJECT_PROPERTY("maxThreads", &WebSocketServer::setMaxThreads)
BEEEON_OBJECT_PROPERTY("threadIdleTime", &WebSocketServer::setThreadIdleTime)
BEEEON_OBJECT_PROPERTY("eventsExecutor", &WebSocketServer::setEventsExecutor)
BEEEON_OBJECT_PROPERTY("listeners", &WebSocketServer::registerListener)
BEEEON_OBJECT_END(BeeeOn, WebSocketServer)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

WebSocketServer::WebSocketServer():
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

void WebSocketServer::setVerifierFactory(SocketGatewayPeerVerifierFactory::Ptr factory)
{
	m_verifierFactory = factory;
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
			m_gatewayService,
			m_verifierFactory
		));

	return new HTTPServer(factory, pool(), createSocket(), new HTTPServerParams);
}

ServerSocket WebSocketServer::createSocket()
{
	return m_sslConfig.isNull() ? ServerSocket(bindAddress(), backlog())
			: SecureServerSocket(bindAddress(), backlog(), m_sslConfig->context());
}

void WebSocketServer::doStart()
{
	m_server = createServer();
	m_server->start();
}

void WebSocketServer::doStop()
{
	m_server->stop();
}
