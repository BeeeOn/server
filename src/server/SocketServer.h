#ifndef BEEEON_SOCKET_SERVER_H
#define BEEEON_SOCKET_SERVER_H

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerParams.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/Context.h>

#include "loop/StoppableLoop.h"
#include "server/ServerListener.h"
#include "ssl/SSLServer.h"
#include "util/EventSource.h"
#include "util/Loggable.h"

namespace BeeeOn {

class SocketServerConnectionFactory : public Poco::Net::TCPServerConnectionFactory {
public:
	typedef Poco::SharedPtr<SocketServerConnectionFactory> Ptr;

	virtual Poco::Net::TCPServerConnection *createConnection(
			const Poco::Net::StreamSocket &socket) = 0;
};

class SocketServer : public StoppableLoop, protected Loggable {
public:
	SocketServer();

	void setName(const std::string &name);
	void setSSLConfig(SSLServer::Ptr config);
	void setHost(const std::string &host);
	void setPort(int port);
	void setBacklog(int backlog);
	void setFactory(SocketServerConnectionFactory::Ptr factory);
	void setMaxThreads(int count);
	void setMaxQueued(int count);
	void setThreadIdleTime(const Poco::Timespan &time);
	void setThreadPriority(const std::string &priority);

	void setEventsExecutor(AsyncExecutor::Ptr executor);
	void registerListener(ServerListener::Ptr listener);

	void start() override;
	void stop() override;

protected:
	Poco::Net::TCPServer *createServer();

private:
	std::string m_name;
	Poco::Net::SocketAddress m_bind;
	int m_backlog;
	SSLServer::Ptr m_sslConfig;
	SocketServerConnectionFactory::Ptr m_factory;
	Poco::Net::TCPServerParams::Ptr m_tcpParams;
	Poco::SharedPtr<Poco::Net::TCPServer> m_server;
	EventSource<ServerListener> m_eventSource;
};

}

#endif
