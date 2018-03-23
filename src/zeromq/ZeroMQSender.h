#pragma once

#include <string>

#include <Poco/SharedPtr.h>
#include <Poco/ThreadLocal.h>

#include "zeromq/ZeroMQContext.h"
#include "util/Loggable.h"

namespace zmq {

class message_t;
class socket_t;

}

namespace BeeeOn {

/**
 * @brief ZeroMQSender represents a source of messages. It can only
 * send message but it is unable to receive any. Thus, it supports
 * only the following ZeroMQ socket types:
 *
 * - PUB - binds to the endpoint (unreliable in principle),
 * - PUSH - connects to the endpoint,
 * - PAIR - connects to the endpoint,
 * - DEALER - binds to the endpoint.
 *
 */
class ZeroMQSender : protected Loggable {
public:
	typedef Poco::SharedPtr<ZeroMQSender> Ptr;

	/**
	 * Prepare sender for communication. Create socket(s) and
	 * establish connection with the remote endpoint.
	 */
	virtual void prepare(const std::string &endpoint) = 0;

	/**
	 * Send the given ZeroMQ message to the remote endpoint.
	 */
	bool send(zmq::message_t &message);

	/**
	 * Create ZeroMQSender instance based on the type.
	 */
	static ZeroMQSender::Ptr create(ZeroMQContext::Ptr context, int type);

protected:
	ZeroMQSender();

	/**
	 * Perform socket->connect and convert low-level errors to
	 * higher-level exceptions.
	 */
	void connect(Poco::SharedPtr<zmq::socket_t> socket,
			const std::string &endpoint);

	/**
	 * Perform socket->bind and convert low-level errors to
	 * higher-level exceptions.
	 */
	void bind(Poco::SharedPtr<zmq::socket_t> socket,
			const std::string &endpoint);

	/**
	 * Do the actual socket->send() call in non-blocking mode
	 * and handle error states.
	 *
	 * @return true if send was successful, false when the message
	 * was not sent due to high load (full buffers) or issues with
	 * connections to the remote endpoint.
	 */
	bool sendNoBlock(Poco::SharedPtr<zmq::socket_t> socket,
			zmq::message_t &message) const;

	virtual bool sendImpl(zmq::message_t &message) = 0;
};

namespace ZeroMQInternal {

class ConnectingSender : public ZeroMQSender {
public:
	ConnectingSender(ZeroMQContext::Ptr context, int type);

	void prepare(const std::string &endpoint) override;
	bool sendImpl(zmq::message_t &message) override;

private:
	int m_type;
	ZeroMQContext::Ptr m_context;
	std::string m_endpoint;
	Poco::ThreadLocal<Poco::SharedPtr<zmq::socket_t>> m_socket;
};

class BindingSender : public ZeroMQSender {
public:
	BindingSender(Poco::SharedPtr<zmq::socket_t> socket);

	void prepare(const std::string &endpoint) override;
	bool sendImpl(zmq::message_t &message) override;

private:
	Poco::FastMutex m_lock;
	Poco::SharedPtr<zmq::socket_t> m_socket;
};

}

}
