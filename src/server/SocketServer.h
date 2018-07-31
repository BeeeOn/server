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

#include "server/AbstractServer.h"
#include "ssl/SSLServer.h"

namespace BeeeOn {

class SocketServerConnectionFactory : public Poco::Net::TCPServerConnectionFactory {
public:
	typedef Poco::SharedPtr<SocketServerConnectionFactory> Ptr;

	virtual Poco::Net::TCPServerConnection *createConnection(
			const Poco::Net::StreamSocket &socket) = 0;
};

class SocketServer : public AbstractServer {
public:
	SocketServer();

	void setSSLConfig(SSLServer::Ptr config);
	void setFactory(SocketServerConnectionFactory::Ptr factory);
	void setMaxThreads(int count);
	void setMaxQueued(int count);
	void setThreadIdleTime(const Poco::Timespan &time);
	void setThreadPriority(const std::string &priority);

protected:
	Poco::Net::TCPServer *createServer();

	void doStart() override;
	void doStop() override;

private:
	SSLServer::Ptr m_sslConfig;
	SocketServerConnectionFactory::Ptr m_factory;
	Poco::Net::TCPServerParams::Ptr m_tcpParams;
	Poco::SharedPtr<Poco::Net::TCPServer> m_server;
};

}

#endif
