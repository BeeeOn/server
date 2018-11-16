#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Nullable.h>
#include <Poco/NumberFormatter.h>

#include <Poco/Net/NetException.h>

#include "server/WebSocketConnection.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

/// @see https://tools.ietf.org/html/rfc6455#section-5.5
#define CONTROL_PAYLOAD_LIMIT 125
#define FRAME_OP_CONTROL_MASK 0x08

WebSocketConnection::WebSocketConnection(
		const string &id,
		const WebSocket &webSocket,
		SocketReactor &reactor,
		const EnqueueReadable &enqueueReadable,
		const size_t maxFrameSize):
	m_id(id),
	m_webSocket(webSocket),
	m_reactor(reactor),
	m_enqueueReadable(enqueueReadable),
	m_maxFrameSize(maxFrameSize),
	m_readableObserver(*this, &WebSocketConnection::onReadable)
{
	if (logger().information()) {
		logger().information(
			"created connection to " + this->id(),
			__FILE__, __LINE__);
	}
}

WebSocketConnection::~WebSocketConnection()
{
	if (logger().information()) {
		logger().information(
			"destroyed connection to " + id(),
			__FILE__, __LINE__);
	}
}

string WebSocketConnection::id() const
{
	return m_id;
}

void WebSocketConnection::addToReactor() const
{
	m_reactor.addEventHandler(m_webSocket, m_readableObserver);
}

void WebSocketConnection::removeFromReactor() const
{
	m_reactor.removeEventHandler(m_webSocket, m_readableObserver);
}

Nullable<string> WebSocketConnection::receiveFrame()
{
	Buffer<char> buffer(m_maxFrameSize);
	int flags = 0;

	int ret = m_webSocket.receiveFrame(
			buffer.begin(), buffer.size(), flags);

	const int opcode = flags & WebSocket::FRAME_OP_BITMASK;

	if (ret <= 0 || opcode == WebSocket::FRAME_OP_CLOSE)
		throw ConnectionResetException("received opcode CLOSE");

	const size_t length = static_cast<size_t>(ret);

	if (logger().trace()) {
		logger().dump(
			"received frame of size "
			+ to_string(length) + " B ("
			+ NumberFormatter::formatHex(flags, true) + ")"
			+ " from " + id(),
			buffer.begin(),
			length,
			Message::PRIO_TRACE);
	}
	else if (logger().debug()) {
		logger().debug(
			"received frame of size "
			+ to_string(length) + " B ("
			+ NumberFormatter::formatHex(flags, true) + ")"
			+ " from " + id(),
			__FILE__, __LINE__);
	}

	m_stats.read(length);

	if (opcode & FRAME_OP_CONTROL_MASK && length > CONTROL_PAYLOAD_LIMIT) {
		m_stats.lost(length);
		throw ProtocolException("too long payload for a control frame");
	}

	switch (opcode) {
	case WebSocket::FRAME_OP_TEXT:
		if (!(flags & WebSocket::FRAME_FLAG_FIN)) {
			m_stats.lost(length);
			throw ProtocolException("multi-fragment messages are unsupported");
		}

		return string{buffer.begin(), length};

	case WebSocket::FRAME_OP_PING:
		handlePing(buffer, length);
		break;

	default:
		m_stats.lost(length);
		throw ProtocolException("unhandled websocket opcode: "
				+ NumberFormatter::formatHex(opcode, true));
	}

	static const Nullable<string> null;
	return null;
}

void WebSocketConnection::handlePing(const Buffer<char> &request, size_t length)
{
	sendPong(request, length);
}

void WebSocketConnection::sendFrame(const string &buffer, size_t length)
{
	checkBuffer(buffer, length);

	if (logger().trace()) {
		logger().dump(
			"sending frame of size " + to_string(length) + " B"
			+ " to " + id(),
			buffer.data(),
			length,
			Message::PRIO_TRACE);
	}
	else if (logger().debug()) {
		logger().debug(
			"sending frame of size " + to_string(length) + " B"
			+ " to " + id(),
			__FILE__, __LINE__);
	}

	FastMutex::ScopedLock guard(m_sendLock);
	m_webSocket.sendFrame(buffer.data(), length);

	m_stats.written(length);
}

void WebSocketConnection::sendPong(const Buffer<char> &request, size_t length)
{
	checkBuffer(request, length);

	if (logger().trace()) {
		logger().dump(
			"reply PONG frame of size "
			+ to_string(request.size())
			+ " to " + id(),
			request.begin(),
			length,
			Message::PRIO_TRACE);
	}
	else if (logger().debug()) {
		logger().debug(
			"reply PONG frame of size "
			+ to_string(request.size())
			+ " to " + id(),
			__FILE__, __LINE__);
	}

	FastMutex::ScopedLock guard(m_sendLock);
	m_webSocket.sendFrame(
		request.begin(),
		length,
		WebSocket::FRAME_OP_PONG | WebSocket::FRAME_FLAG_FIN);

	m_stats.written(length);
}

void WebSocketConnection::checkOverflow(const size_t size, size_t length) const
{
	if (length > size) {
		throw RangeException(
			"length " + to_string(length)
			+ " is greater than buffer size "
			+ to_string(size));
	}
}

void WebSocketConnection::onReadable(const AutoPtr<ReadableNotification> &notification)
{
	m_enqueueReadable(AutoPtr<WebSocketConnection>(this, true));
}
