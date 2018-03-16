#include <Poco/Error.h>
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <Poco/Net/NetException.h>

#include <zmq.hpp>

#include "zeromq/ZeroMQSender.h"

using namespace std;
using namespace zmq;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;
using namespace BeeeOn::ZeroMQInternal;

ZeroMQSender::ZeroMQSender():
	Loggable(typeid(ZeroMQSender))
{
}

bool ZeroMQSender::send(message_t &message)
{
	if (logger().trace()) {
		logger().dump(
			"sending message of size "
				+ to_string(message.size()),
			message.data(),
			message.size(),
			Message::PRIO_TRACE);
	}
	else if (logger().debug()) {
		logger().debug("sending message of size "
				+ to_string(message.size()),
				__FILE__, __LINE__);
	}

	return sendImpl(message);
}

bool ZeroMQSender::sendNoBlock(
		SharedPtr<socket_t> socket,
		message_t &message) const
{
	if (!socket->send(message, ZMQ_NOBLOCK)) {
		if (zmq_errno() == EAGAIN) {
			if (logger().debug()) {
				logger().debug("send would block, try again later...",
						__FILE__, __LINE__);
			}

			return false;
		}
		else if (zmq_errno() == EHOSTUNREACH) {
			throw HostNotFoundException(
				"zmq send: " + Error::getMessage(EHOSTUNREACH));
		}
	}

	return true;
}

void ZeroMQSender::connect(
		SharedPtr<socket_t> socket,
		const string &endpoint)
{
	if (logger().debug()) {
		logger().debug("connecting to " + endpoint,
				__FILE__, __LINE__);
	}

	try {
		socket->connect(endpoint);
	}
	catch (const zmq::error_t &e) {
		ZeroMQContext::throwFrom(e, "zmq_connect(" + endpoint + ")");
	}
}

void ZeroMQSender::bind(
		SharedPtr<socket_t> socket,
		const string &endpoint)
{
	if (logger().debug()) {
		logger().debug("binding to " + endpoint,
				__FILE__, __LINE__);
	}

	try {
		socket->bind(endpoint);
	}
	catch (const zmq::error_t &e) {
		ZeroMQContext::throwFrom(e, "zmq_bind(" + endpoint + ")");
	}
}

ZeroMQSender::Ptr ZeroMQSender::create(ZeroMQContext::Ptr context, int type)
{
	switch (type) {
	case ZMQ_PUB:
		return new BindingSender(context->createSocket(ZMQ_PUB));
	case ZMQ_PUSH:
		return new ConnectingSender(context, ZMQ_PUSH);
	case ZMQ_PAIR:
		return new ConnectingSender(context, ZMQ_PAIR);
	case ZMQ_DEALER:
		return new BindingSender(context->createSocket(ZMQ_DEALER));
	default:
		throw InvalidArgumentException(
			"unsupported ZeroMQ source socket type: " + to_string(type));
	}
}

ConnectingSender::ConnectingSender(ZeroMQContext::Ptr context, int type):
	m_type(type),
	m_context(context)
{
}

void ConnectingSender::prepare(const string &endpoint)
{
	m_endpoint = endpoint;
}

bool ConnectingSender::sendImpl(message_t &message)
{
	SharedPtr<socket_t> socket = *m_socket;

	if (socket.isNull()) {
		try {
			socket = m_context->createSocket(m_type);
			connect(socket, m_endpoint);
			*m_socket = socket;
		}
		BEEEON_CATCH_CHAIN_ACTION(logger(), return false)
	}

	return sendNoBlock(socket, message);
}

BindingSender::BindingSender(SharedPtr<socket_t> socket):
	m_socket(socket)
{
}

void BindingSender::prepare(const string &endpoint)
{
	FastMutex::ScopedLock guard(m_lock);
	bind(m_socket, endpoint);
}

bool BindingSender::sendImpl(message_t &message)
{
	FastMutex::ScopedLock guard(m_lock);
	return sendNoBlock(m_socket, message);
}
