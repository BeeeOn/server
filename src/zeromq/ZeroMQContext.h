#pragma once

#include <string>

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

namespace zmq {

class socket_t;
class context_t;
class error_t;

}

namespace BeeeOn {

/**
 * @brief ZeroMQContext wraps a zmq::context_t instance and maintains
 * a ZeroMQ configuration for a set of sockets.
 */
class ZeroMQContext {
public:
	typedef Poco::SharedPtr<ZeroMQContext> Ptr;

	ZeroMQContext();

	void setIOThreads(int count);
	void setMaxMessageSize(int bytes);
	void setMaxSocketsCount(int count);
	void setIPv6(bool enable);
	void setSocketLinger(const Poco::Timespan &timeout);

	Poco::SharedPtr<zmq::socket_t> createSocket(int type);

	static int parseSocketType(const std::string &name);
	static std::string socketTypeName(int type);

	static void throwFrom(
		const zmq::error_t &e,
		const std::string &text);

private:
	Poco::SharedPtr<zmq::context_t> m_context;
	int m_socketLinger;
};

}
