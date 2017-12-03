#pragma once

#include <Poco/Net/TCPServerConnection.h>

namespace BeeeOn {

/**
 * @brief FailingTCPServerConnection is intended for connections that
 * cannot be handled anyway because the client has closed her connection.
 * It should avoid issues with Poco::Net::TCPServer that seems to be
 * stuck when the particular TCPServerConnectionFactory returns nullptr
 * instead of a working connection. The FailingTCPServerConnection throws
 * the Poco::Net::ConnectionAbortedException.
 */
class FailingTCPServerConnection : public Poco::Net::TCPServerConnection {
public:
	FailingTCPServerConnection(const Poco::Net::StreamSocket &socket);

	void run() override;
};

}
