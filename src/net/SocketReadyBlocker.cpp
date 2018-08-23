#include <Poco/Exception.h>
#include <Poco/FIFOBuffer.h>
#include <Poco/Logger.h>
#include <Poco/NumberFormatter.h>
#include <Poco/NumberParser.h>
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
BEEEON_OBJECT_PROPERTY("request", &SocketReadyBlocker::setRequest)
BEEEON_OBJECT_PROPERTY("responseHeader", &SocketReadyBlocker::setResponseHeader)
BEEEON_OBJECT_PROPERTY("responseBody", &SocketReadyBlocker::setResponseBody)
BEEEON_OBJECT_PROPERTY("responseSize", &SocketReadyBlocker::setResponseSize)
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

vector<uint8_t> SocketReadyBlocker::parsePayload(const list<string> &payload)
{
	vector<uint8_t> bytes;

	for (const auto &hex : payload) {
		const auto byte = NumberParser::parseHex(hex);

		if (byte < 0 || byte > 255)
			throw InvalidArgumentException("payload must contain only values in range 00..FF");

		bytes.emplace_back(static_cast<uint8_t>(byte));
	}

	return bytes;
}

string SocketReadyBlocker::payloadToString(const vector<uint8_t> &bytes)
{
	string payload;

	for (auto it = bytes.begin(); it != bytes.end(); ++it) {
		if (it == bytes.begin())
			payload += NumberFormatter::formatHex(*it, 2);
		else
			payload += " " + NumberFormatter::formatHex(*it, 2);
	}

	return payload;
}

void SocketReadyBlocker::setRequest(const list<string> &payload)
{
	m_request = parsePayload(payload);
}

void SocketReadyBlocker::setResponseHeader(const list<string> &payload)
{
	m_responseHeader = parsePayload(payload);
}

void SocketReadyBlocker::setResponseBody(const list<string> &payload)
{
	m_responseBody = parsePayload(payload);
}

void SocketReadyBlocker::setResponseSize(int size)
{
	if (size < 0)
		m_responseSize.clear();
	else
		m_responseSize = size;
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
	}
	BEEEON_CATCH_CHAIN_ACTION(logger(),
		return false)

	try {
		if (!m_request.empty())
			sendRequest(socket);
		if (!m_responseSize.isNull() || !m_responseHeader.empty() || !m_responseBody.empty())
			checkResponse(socket);
	}
	BEEEON_CATCH_CHAIN_ACTION(logger(),
		return false)

	logger().notice("remote " + remote() + " seems to be up");
	return true;
}

void SocketReadyBlocker::sendRequest(StreamSocket &socket) const
{
	if (logger().trace()) {
		logger().dump(
			"sending payload of size " + to_string(m_request.size()) + " B",
			m_request.data(),
			m_request.size(),
			Message::PRIO_TRACE);
	}
	else if (logger().debug()) {
		logger().debug(
			"sending payload of size " + to_string(m_request.size()) + " B",
			__FILE__, __LINE__);
	}

	socket.sendBytes(m_request.data(), m_request.size());
}

void SocketReadyBlocker::checkResponse(StreamSocket &socket) const
{
	FIFOBuffer buffer(1024);
	const auto ret = socket.receiveBytes(buffer);
	poco_assert(ret >= 0);

	const vector<uint8_t> response = {buffer.begin(), buffer.begin() + ret};

	if (logger().trace()) {
		logger().dump(
			"received payload of size "
				+ to_string(response.size()) + " B",
			response.data(),
			response.size(),
			Message::PRIO_TRACE);
	}
	else if (logger().debug()) {
		logger().debug(
			"received payload of size "
				+ to_string(response.size()) + " B",
			__FILE__, __LINE__);
	}

	if (!m_responseSize.isNull()) {
		if (response.size() != m_responseSize) {
			throw ProtocolException(
				"received "
				+ to_string(response.size()) + " B but expected "
				+ to_string(m_responseSize) + " B");
		}

		if (logger().debug())
			logger().debug("received payload size matches", __FILE__, __LINE__);
	}

	if (!m_responseHeader.empty()) {
		if (response.size() < m_responseHeader.size()) {
			throw ProtocolException("response is shorter ("
					+ to_string(response.size())
					+ " B) than expected header ("
					+ to_string(m_responseHeader.size()) + " B)");
		}

		const vector<uint8_t> slice =
			{response.begin(), response.begin() + m_responseHeader.size()};

		if (slice != m_responseHeader) {
			throw ProtocolException(
				"received unexpected header: "
				+ payloadToString(slice));
		}

		if (logger().debug())
			logger().debug("received header matches", __FILE__, __LINE__);
	}

	if (!m_responseBody.empty()) {
		if (response.size() < m_responseHeader.size() + m_responseBody.size()) {
			throw ProtocolException("response is shorter ("
					+ to_string(response.size())
					+ " B) than expected header and body ("
					+ to_string(m_responseHeader.size() + m_responseBody.size())
					+ " B)");
		}

		const vector<uint8_t> slice =
			{response.begin() + m_responseHeader.size(), response.end()};

		if (slice != m_responseBody) {
			throw ProtocolException(
				"received unexpected body: "
				+ payloadToString(slice));
		}

		if (logger().debug())
			logger().debug("received body matches", __FILE__, __LINE__);
	}
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
