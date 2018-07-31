#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Net/SecureServerSocket.h>

#include "di/Injectable.h"
#include "gws/WebSocketServer.h"
#include "gws/WebSocketRequestHandler.h"

BEEEON_OBJECT_BEGIN(BeeeOn, WebSocketServer)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("sslConfig", &WebSocketServer::setSSLConfig)
BEEEON_OBJECT_PROPERTY("requestHandlerFactory", &WebSocketServer::setRequestHandlerFactory)
BEEEON_OBJECT_PROPERTY("name", &WebSocketServer::setName)
BEEEON_OBJECT_PROPERTY("host", &WebSocketServer::setHost)
BEEEON_OBJECT_PROPERTY("port", &WebSocketServer::setPort)
BEEEON_OBJECT_PROPERTY("backlog", &WebSocketServer::setBacklog)
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

WebSocketServer::WebSocketServer()
{
}

void WebSocketServer::setSSLConfig(SSLServer::Ptr config)
{
	m_sslConfig = config;
}

void WebSocketServer::setRequestHandlerFactory(WebSocketRequestHandlerFactory::Ptr factory)
{
	m_factory = factory;
}

Poco::Net::HTTPServer *WebSocketServer::createServer()
{
	return new HTTPServer(m_factory, pool(), createSocket(), new HTTPServerParams);
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
