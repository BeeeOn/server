#ifndef BEEEON_SOCKET_SERVER_H
#define BEEEON_SOCKET_SERVER_H

#include <Poco/SharedPtr.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerParams.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/Context.h>

#include "server/Server.h"

namespace BeeeOn {

class SSLServer;

class SocketServer : public Server {
public:
	SocketServer();

	void setSSLConfig(SSLServer *config);
	void setPort(int port);
	void setBacklog(int backlog);
	void setFactory(Poco::Net::TCPServerConnectionFactory::Ptr factory);
	void setTCPParams(const Poco::Net::TCPServerParams::Ptr params);

	void start();
	void stop();

	static SocketServer *create(
		Poco::Net::TCPServerConnectionFactory::Ptr factory,
		SSLServer *sslConfig,
		Poco::UInt16 port);

protected:
	Poco::Net::TCPServer *createServer();

private:
	unsigned int m_port;
	int m_backlog;
	SSLServer *m_sslConfig;
	Poco::Net::TCPServerConnectionFactory::Ptr m_factory;
	Poco::Net::TCPServerParams::Ptr m_tcpParams;
	Poco::SharedPtr<Poco::Net::TCPServer> m_server;
};

}

#endif
