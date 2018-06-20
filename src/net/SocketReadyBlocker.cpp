#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/StreamSocket.h>

#include "di/Injectable.h"
#include "net/SocketReadyBlocker.h"

BEEEON_OBJECT_BEGIN(BeeeOn, SocketReadyBlocker)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("host", &SocketReadyBlocker::setHost)
BEEEON_OBJECT_PROPERTY("port", &SocketReadyBlocker::setPort)
BEEEON_OBJECT_PROPERTY("timeout", &SocketReadyBlocker::setTimeout)
BEEEON_OBJECT_PROPERTY("interval", &SocketReadyBlocker::setInterval)
BEEEON_OBJECT_END(BeeeOn, SocketReadyBlocker)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

static const Timespan CONNECT_TIMEOUT = 100 * Timespan::MILLISECONDS;

SocketReadyBlocker::SocketReadyBlocker():
	m_host("127.0.0.1"),
	m_port(-1),
	m_timeout(5 * Timespan::SECONDS),
	m_interval(1 * Timespan::SECONDS)
{
}

void SocketReadyBlocker::setHost(const string &host)
{
	m_host = host;
}

void SocketReadyBlocker::setPort(int port)
{
	if (port < 0)
		throw InvalidArgumentException("port most not be negative");

	m_port = port;
}

void SocketReadyBlocker::setTimeout(const Timespan &timeout)
{
	m_timeout = timeout;
}

void SocketReadyBlocker::setInterval(const Timespan &interval)
{
	if (interval <= 1 * Timespan::MILLISECONDS)
		throw InvalidArgumentException("interval must be at least 1 ms");

	m_interval = interval;
}

void SocketReadyBlocker::checkTimeout(const Clock &clock) const
{
	if (m_timeout < 0)
		return;

	if (clock.isElapsed(m_timeout.totalMicroseconds()))
		throw TimeoutException("remote " + remote() + " seems to be down");
}

string SocketReadyBlocker::remote() const
{
	return m_host + ":" + to_string(m_port);
}

bool SocketReadyBlocker::ping() const
{
	StreamSocket socket;

	if (logger().debug()) {
		logger().debug(
			"checking whether remote " + remote() + " is up",
			__FILE__, __LINE__);
	}

	try {
		socket.connect({m_host, (uint16_t) m_port}, CONNECT_TIMEOUT);

		logger().notice("remote " + remote() + " seems to be up");
		return true;
	}
	BEEEON_CATCH_CHAIN(logger())

	return false;
}

void SocketReadyBlocker::start()
{
	const Clock started;

	if (m_port < 0)
		throw IllegalStateException("port has not been configured");

	logger().notice("waiting for " + remote());

	if (ping())
		return;

	while (!m_event.tryWait(m_interval.totalMilliseconds())) {
		checkTimeout(started);

		if (ping())
			return;
	}

	logger().notice("stopping blocker waiting for " + remote());
}

void SocketReadyBlocker::stop()
{
	m_event.set();
}
