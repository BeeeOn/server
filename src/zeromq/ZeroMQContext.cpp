#include <Poco/Error.h>
#include <Poco/Exception.h>

#include <zmq.hpp>

#include "di/Injectable.h"
#include "zeromq/ZeroMQContext.h"

BEEEON_OBJECT_BEGIN(BeeeOn, ZeroMQContext)
BEEEON_OBJECT_PROPERTY("ioThreads", &ZeroMQContext::setIOThreads)
BEEEON_OBJECT_PROPERTY("maxMessageSize", &ZeroMQContext::setMaxMessageSize)
BEEEON_OBJECT_PROPERTY("maxSocketsCount", &ZeroMQContext::setMaxSocketsCount)
BEEEON_OBJECT_PROPERTY("ipv6", &ZeroMQContext::setIPv6)
BEEEON_OBJECT_PROPERTY("socketLinger", &ZeroMQContext::setSocketLinger)
BEEEON_OBJECT_END(BeeeOn, ZeroMQContext)

using namespace std;
using namespace zmq;
using namespace Poco;
using namespace BeeeOn;

#ifndef ZMQ_MAX_MSGSZ
#define ZMQ_MAX_MSGSZ 5 // from zeromq 4.2.2
#endif

#define SOCKET_LINGER_MS 1000

ZeroMQContext::ZeroMQContext() try :
	m_context(new context_t(1)),
	m_socketLinger(SOCKET_LINGER_MS)
{
}
catch (const zmq::error_t &e) {
	ZeroMQContext::throwFrom(e, "zmq_ctx_new(1)");
}

void ZeroMQContext::setIOThreads(int count)
{
	if (count < 1)
		throw InvalidArgumentException("ioThreads must be at least 1");

	zmq_ctx_set(m_context, ZMQ_IO_THREADS, count);
}

void ZeroMQContext::setMaxMessageSize(int bytes)
{
	if (bytes < 1)
		throw InvalidArgumentException("maxMessageSize must be at least 1");

	zmq_ctx_set(m_context, ZMQ_MAX_MSGSZ, bytes);
}

void ZeroMQContext::setMaxSocketsCount(int count)
{
	if (count < 1)
		throw InvalidArgumentException("maxSocketsCount must be at least 1");

	zmq_ctx_set(m_context, ZMQ_MAX_SOCKETS, count);
}

void ZeroMQContext::setIPv6(bool enable)
{
	zmq_ctx_set(m_context, ZMQ_IPV6, enable ? 1 : 0);
}

void ZeroMQContext::setSocketLinger(const Timespan &timeout)
{
	if (timeout > 0 && timeout < 1 * Timespan::MILLISECONDS)
		throw InvalidArgumentException("socketLinger must be at least 1 ms");

	m_socketLinger = timeout < 0 ? -1 : timeout.totalMilliseconds();
}

SharedPtr<socket_t> ZeroMQContext::createSocket(int type)
{
	SharedPtr<socket_t> socket;

	try {
		socket = new socket_t(*m_context, type);
	}
	catch (const zmq::error_t &e) {
		throwFrom(e, "zmq_socket(" + socketTypeName(type) + ")");
	}

	try {
		socket->setsockopt(ZMQ_LINGER, m_socketLinger);
	}
	catch (const zmq::error_t &e) {
		throwFrom(e, "zmq_setsockopt(" + to_string(m_socketLinger) + ")");
	}

	return socket;
}

int ZeroMQContext::parseSocketType(const string &name)
{
	if (!icompare(name, "pub"))
		return ZMQ_PUB;
	if (!icompare(name, "push"))
		return ZMQ_PUSH;
	if (!icompare(name, "pair"))
		return ZMQ_PAIR;
	if (!icompare(name, "dealer"))
		return ZMQ_DEALER;
	if (!icompare(name, "req"))
		return ZMQ_REQ;
	if (!icompare(name, "rep"))
		return ZMQ_REP;
	if (!icompare(name, "router"))
		return ZMQ_ROUTER;

	throw InvalidArgumentException(
		"unsupported ZeroMQ socket type: " + name);
}

string ZeroMQContext::socketTypeName(int type)
{
	switch (type) {
	case ZMQ_PUB:
		return "pub";
	case ZMQ_PUSH:
		return "push";
	case ZMQ_PAIR:
		return "pair";
	case ZMQ_DEALER:
		return "dealer";
	case ZMQ_REQ:
		return "req";
	case ZMQ_REP:
		return "rep";
	case ZMQ_ROUTER:
		return "router";
	case ZMQ_PULL:
		return "pull";
	default:
		throw InvalidArgumentException(
			"invalid ZeroMQ type: " + to_string(type));
	}
}

void ZeroMQContext::throwFrom(
	const zmq::error_t &e,
	const string &text)
{
	switch (e.num()) {
	case EINVAL:
	case ENOCOMPATPROTO:
	case EPROTONOSUPPORT:
		throw InvalidArgumentException(text
			+ ": " + Error::getMessage(e.num()));

	case ETERM:
	case ENOTSOCK:
	case EMTHREAD:
		throw IllegalStateException(text
			+ ": " + Error::getMessage(e.num()));

	case EINTR:
		throw SignalException(text
			+ ": " + Error::getMessage(e.num()));

	default:
		throw IOException(text
			+ ": " + Error::getMessage(e.num()));
	}

}
