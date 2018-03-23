#pragma once

#include <deque>
#include <string>

#include <Poco/AtomicCounter.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "baf/EventPublisher.h"
#include "util/Loggable.h"
#include "zeromq/ZeroMQContext.h"
#include "zeromq/ZeroMQSender.h"

namespace zmq {

class context_t;
class message_t;

}

namespace BeeeOn {

/**
 * @brief ZeroMQPublisher implements the EventPublisher interface
 * to deliver any application events over the ZeroMQ to some extenal
 * system for further processing. If sending of an event fails, the
 * such event is dropped.
 */
class ZeroMQPublisher :
	public BeeeOn::Automation::EventPublisher,
	Loggable {
public:
	ZeroMQPublisher();

	void setContext(ZeroMQContext::Ptr context);
	void setEndpoint(const std::string &endpoint);
	void setType(const std::string &type);

	void publish(const std::string &message) override;

	void zmqInitialize();

private:
	std::string m_endpoint;
	int m_type;
	Poco::AtomicCounter m_sent;
	Poco::AtomicCounter m_dropped;
	Poco::FastMutex m_lock;
	ZeroMQContext::Ptr m_context;
	ZeroMQSender::Ptr m_source;
};

}
