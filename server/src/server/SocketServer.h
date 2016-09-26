#ifndef BEEEON_SOCKET_SERVER_H
#define BEEEON_SOCKET_SERVER_H

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerParams.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>

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


private:
	Poco::Net::TCPServer m_server;
};

}

#endif
