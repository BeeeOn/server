#ifndef BEEEON_WEBSOCKET_SERVER
#define BEEEON_WEBSOCKET_SERVER

#include <Poco/SharedPtr.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>

#include "server/AbstractServer.h"
#include "gws/GWRequestHandler.h"
#include "ssl/SSLServer.h"

namespace BeeeOn {

/**
 * @brief WebSocketServer is responsible for handling new WebSocket connections
 * from gateways, registering them and passing them to the GatewayCommunicator.
 *
 * It uses the Poco::Net::HTTPServer framework, and it is multi-thread server
 * using thread pool. The WebSocketServer can run unsecured or secured by SSL/TLS.
 *
 * For each incoming connection is created the GWRequestHandler by the
 * GWRequestHandlerFactory, which handle() method is invoked in thread
 * pool. The WebSocket connection is there created, gateway is registered and
 * then the connection is passed to the GatewayCommunicator. After the connection
 * is added to the communicator, handler is destroyed and serving thread is
 * returned to the thread pool.
 */
class WebSocketServer :
		public AbstractServer,
		public HavingThreadPool {
public:
	WebSocketServer();

	void setSSLConfig(SSLServer::Ptr config);
	void setRequestHandlerFactory(GWRequestHandlerFactory::Ptr factory);

	/**
	 * @brief Set max message size just for register message.
	 */
	void setMaxMessageSize(int size);

protected:
	void doStart() override;
	void doStop() override;

	Poco::Net::HTTPServer *createServer();

private:
	Poco::Net::ServerSocket createSocket();

private:
	SSLServer::Ptr m_sslConfig;
	Poco::SharedPtr<Poco::Net::HTTPServer> m_server;
	GWRequestHandlerFactory::Ptr m_factory;
};

}

#endif
