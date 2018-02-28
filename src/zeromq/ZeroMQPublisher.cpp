#include <Poco/Error.h>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/String.h>
#include <Poco/Net/NetException.h>

#include <zmq.hpp>

#include "di/Injectable.h"
#include "util/Occasionally.h"
#include "zeromq/ZeroMQPublisher.h"

BEEEON_OBJECT_BEGIN(BeeeOn, ZeroMQPublisher)
BEEEON_OBJECT_CASTABLE(BeeeOn::Automation::EventPublisher)
BEEEON_OBJECT_PROPERTY("context", &ZeroMQPublisher::setContext)
BEEEON_OBJECT_PROPERTY("endpoint", &ZeroMQPublisher::setEndpoint)
BEEEON_OBJECT_PROPERTY("type", &ZeroMQPublisher::setType)
BEEEON_OBJECT_HOOK("done", &ZeroMQPublisher::zmqInitialize)
BEEEON_OBJECT_END(BeeeOn, ZeroMQPublisher)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

ZeroMQPublisher::ZeroMQPublisher():
	m_endpoint(""),
	m_type(ZMQ_PUB)
{
}

void ZeroMQPublisher::setContext(ZeroMQContext::Ptr context)
{
	m_context = context;
}

void ZeroMQPublisher::setEndpoint(const string &endpoint)
{
	m_endpoint = endpoint;
}

void ZeroMQPublisher::setType(const string &name)
{
	const int type = ZeroMQContext::parseSocketType(name);

	switch (type) {
	case ZMQ_PUB:
	case ZMQ_PUSH:
	case ZMQ_PAIR:
	case ZMQ_DEALER:
		m_type = type;
		break;

	default:
		throw InvalidArgumentException(
			"unsupported ZeroMQ socket type: "
			+ ZeroMQContext::socketTypeName(type));
	}
}

void ZeroMQPublisher::zmqInitialize()
{
	if (m_endpoint.empty())
		throw IllegalStateException("no ZeroMQ endpoint specified");

	logger().notice("initializing ZeroMQ socket "
			+ ZeroMQContext::socketTypeName(m_type)
			+ " to "
			+ m_endpoint,
		__FILE__, __LINE__);

	m_source = ZeroMQSender::create(m_context, m_type);
	m_source->prepare(m_endpoint);
}

void ZeroMQPublisher::publish(const string &message)
{
	static Occasionally occasionally;

	occasionally.execute([&]() {
		logger().information("publishing stats: "
				+ to_string(m_sent)
				+ "/"
				+ to_string(m_dropped),
				__FILE__, __LINE__);
	});

	zmq::message_t msg(message.data(), message.size());

	try {
		if (m_source->send(msg)) {
			++m_sent;
		}
		else {
			if (logger().debug()) {
				logger().debug("message was not sent...",
					__FILE__, __LINE__);
			}

			++m_dropped;
		}
	}
	BEEEON_CATCH_CHAIN_ACTION(logger(), ++m_dropped)
}
