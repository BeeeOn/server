#ifndef BEEEON_SOCKET_RPC_CONNECTOR_H
#define BEEEON_SOCKET_RPC_CONNECTOR_H

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "rpc/RPCConnector.h"

namespace Poco {
namespace Net {

class StreamSocket;
class SocketStream;

}
}

namespace BeeeOn {

class SocketRPCConnector: public RPCConnector {
public:
	SocketRPCConnector();

	void setHost(const std::string &host)
	{
		m_host = host;
	}

	void setPort(int port)
	{
		m_port = port;
	}

	void setReceiveTimeout(const Poco::Timespan &timeout)
	{
		m_receiveTimeout = timeout;
	}

	void open() override;
	std::ostream &send() override;
	void flush() override;
	std::istream &receive() override;
	void close() override;

private:
	std::string m_host;
	Poco::UInt16 m_port;
	Poco::Timespan m_receiveTimeout;
	Poco::SharedPtr<Poco::Net::StreamSocket> m_socket;
	Poco::SharedPtr<Poco::Net::SocketStream> m_socketStream;
};

}

#endif
