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
	m_bind("127.0.0.1:0"),
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

void WebSocketServer::setVerifierFactory(SocketGatewayPeerVerifierFactory::Ptr factory)
{
	m_verifierFactory = factory;
}

void WebSocketServer::setName(const string &name)
{
	m_name = name;
}

void WebSocketServer::setHost(const string &host)
{
	m_bind = SocketAddress(host, m_bind.port());
}

void WebSocketServer::setPort(int port)
{
	if (port < 0)
		throw InvalidArgumentException("port must be non-negative");

	m_bind = SocketAddress(m_bind.host(), port);
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

void WebSocketServer::setEventsExecutor(AsyncExecutor::Ptr executor)
{
	m_eventSource.setAsyncExecutor(executor);
}

void WebSocketServer::registerListener(ServerListener::Ptr listener)
{
	m_eventSource.addListener(listener);
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
	return m_sslConfig.isNull() ? ServerSocket(m_bind, m_backlog)
			: SecureServerSocket(m_bind, m_backlog, m_sslConfig->context());
}

void WebSocketServer::start()
{
	m_server = createServer();

	logger().information("starting WebSocket server on port: "
			+ to_string(m_server->port()), __FILE__, __LINE__);

	m_server->start();

	const ServerEvent e = {m_bind.toString(), m_name};
	m_eventSource.fireEvent(e, &ServerListener::onUp);
}

void WebSocketServer::stop()
{
	logger().information("stopping WebSocket server", __FILE__, __LINE__);
	m_server->stop();

	const ServerEvent e = {m_bind.toString(), m_name};
	m_eventSource.fireEvent(e, &ServerListener::onDown);
}
