#ifndef BEEEON_SOCKET_SERVER_H
#define BEEEON_SOCKET_SERVER_H

#include <Poco/SharedPtr.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerParams.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/Context.h>

#include "loop/StoppableLoop.h"
#include "util/Loggable.h"

namespace BeeeOn {

class SSLServer;

class SocketServerConnectionFactory : public Poco::Net::TCPServerConnectionFactory {
public:
	typedef Poco::SharedPtr<SocketServerConnectionFactory> Ptr;

	virtual Poco::Net::TCPServerConnection *createConnection(
			const Poco::Net::StreamSocket &socket) = 0;
};

class SocketServer : public StoppableLoop, protected Loggable {
public:
	SocketServer();

	void setSSLConfig(SSLServer *config);
	void setPort(int port);
	void setBacklog(int backlog);
	void setFactory(SocketServerConnectionFactory::Ptr factory);
	void setMaxThreads(int count);
	void setMaxQueued(int count);
	void setThreadIdleTime(int seconds);
	void setThreadPriority(const std::string &priority);

	void start() override;
	void stop() override;

protected:
	Poco::Net::TCPServer *createServer();

private:
	unsigned int m_port;
	int m_backlog;
	SSLServer *m_sslConfig;
	SocketServerConnectionFactory::Ptr m_factory;
	Poco::Net::TCPServerParams::Ptr m_tcpParams;
	Poco::SharedPtr<Poco::Net::TCPServer> m_server;
};

}

#endif
