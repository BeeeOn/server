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

class SocketServer : public Server {
public:
	SocketServer(
		Poco::Net::TCPServerConnectionFactory::Ptr factory,
		const Poco::Net::ServerSocket &socket,
		Poco::Net::TCPServerParams::Ptr params);
	void start();
	void stop();

	static SocketServer *createDefault(
		Poco::Net::TCPServerConnectionFactory::Ptr factory,
		Poco::UInt16 port);

	static SocketServer *createSecure(
		Poco::Net::TCPServerConnectionFactory::Ptr factory,
		Poco::Net::Context::Ptr context,
		Poco::UInt16 port = 443);

private:
	Poco::Net::TCPServerConnectionFactory::Ptr m_factory;
	Poco::Net::ServerSocket m_socket;
	Poco::Net::TCPServerParams::Ptr m_tcpParams;
	Poco::SharedPtr<Poco::Net::TCPServer> m_server;
};

}

#endif
