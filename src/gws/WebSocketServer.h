#ifndef BEEEON_WEBSOCKET_SERVER
#define BEEEON_WEBSOCKET_SERVER

#include <Poco/SharedPtr.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>

#include "gws/GatewayCommunicator.h"
#include "gws/SocketGatewayPeerVerifierFactory.h"
#include "loop/StoppableLoop.h"
#include "server/ServerListener.h"
#include "service/GWSGatewayService.h"
#include "ssl/SSLServer.h"
#include "util/EventSource.h"

namespace BeeeOn {

/**
 * @brief WebSocketServer is responsible for handling new WebSocket connections
 * from gateways, registering them and passing them to the GatewayCommunicator.
 *
 * It uses the Poco::Net::HTTPServer framework, and it is multi-thread server
 * using thread pool. The WebSocketServer can run unsecured or secured by SSL/TLS.
 *
 * For each incoming connection is created the WebSocketRequestHandler by the
 * WebSocketRequestHandlerFactory, which handle() method is invoked in thread
 * pool. The WebSocket connection is there created, gateway is registered and
 * then the connection is passed to the GatewayCommunicator. After the connection
 * is added to the communicator, handler is destroyed and serving thread is
 * returned to the thread pool.
 */
class WebSocketServer :
		public StoppableLoop,
		public HavingThreadPool {
public:
	WebSocketServer();

	void start() override;
	void stop() override;

	void setGatewayCommunicator(GatewayCommunicator::Ptr communicator);
	void setSSLConfig(SSLServer::Ptr config);
	void setGatewayService(GWSGatewayService::Ptr service);
	void setVerifierFactory(SocketGatewayPeerVerifierFactory::Ptr factory);

	void setName(const std::string &name);
	void setHost(const std::string &host);
	void setPort(int port);
	void setBacklog(int backlog);

	/**
	 * @brief Set max message size just for register message.
	 */
	void setMaxMessageSize(int size);

	void setEventsExecutor(AsyncExecutor::Ptr executor);
	void registerListener(ServerListener::Ptr listener);

protected:
	Poco::Net::HTTPServer *createServer();

private:
	Poco::Net::ServerSocket createSocket();

private:
	std::string m_name;
	Poco::Net::SocketAddress m_bind;
	int m_backlog;
	size_t m_maxMessageSize;

	SSLServer::Ptr m_sslConfig;
	GatewayCommunicator::Ptr m_gatewayCommunicator;
	GWSGatewayService::Ptr m_gatewayService;
	Poco::SharedPtr<Poco::Net::HTTPServer> m_server;
	SocketGatewayPeerVerifierFactory::Ptr m_verifierFactory;
	EventSource<ServerListener> m_eventSource;
};

}

#endif
