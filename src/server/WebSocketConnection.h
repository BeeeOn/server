#pragma once

#include <functional>

#include <Poco/AutoPtr.h>
#include <Poco/Buffer.h>
#include <Poco/Mutex.h>
#include <Poco/RefCountedObject.h>

#include <Poco/Net/WebSocket.h>

#include "io/IOStats.h"
#include "util/Loggable.h"

namespace BeeeOn {

class WebSocketConnection :
	public Poco::RefCountedObject,
	protected Loggable {
public:
	typedef Poco::AutoPtr<WebSocketConnection> Ptr;

	WebSocketConnection(
		const std::string &id,
		const Poco::Net::WebSocket &webSocket,
		const size_t maxFrameSize);

	virtual ~WebSocketConnection();

	Poco::Net::WebSocket socket() const;

protected:
	/**
	 * @brief To be called by the derived class in its constructor.
	 * Inside this function it is already clear, of what type is the
	 * pointer _this_.
	 */
	void afterConstructed() const;

	/**
	 * @returns string identification of the connection that used
	 * for logging and similar purposes.
	 */
	std::string id() const;

	/**
	 * @brief Receive a frame from the associated websocket.
	 * @returns null if the received frame was a control frame,
	 * otherwise it returns the received payload.
	 */
	Poco::Nullable<std::string> receiveFrame();

	/**
	 * @brief Handle incoming ping request. The default action is
	 * to reply pong.
	 */
	virtual void handlePing(const Poco::Buffer<char> &request, size_t length);

	/**
	 * @brief Send the given buffer of the given length via
	 * the associated websocket as a single frame.
	 */
	void sendFrame(const std::string &buffer, size_t length);

	/**
	 * @brief Sends back the PONG frame. Such frame must contain the same
	 * application data as it was received.
	 *
	 * @see https://tools.ietf.org/html/rfc6455#section-5.5.2
	 */
	void sendPong(const Poco::Buffer<char> &request, size_t length);

	/**
	 * @brief Check that the length does not overflow hte buffer.
	 * @throws Poco::RangeException in case of buffer overflow
	 */
	template <typename Buf>
	void checkBuffer(const Buf &buffer, size_t length) const
	{
		checkOverflow(buffer.size(), length);
	}

private:
	/**
	 * @brief Implementation of buffer overflow test.
	 * @see checkBuffer()
	 */
	void checkOverflow(const size_t bufferSize, size_t length) const;

protected:
	IOStats m_stats;

private:
	std::string m_id;
	Poco::Net::WebSocket m_webSocket;
	size_t m_maxFrameSize;
	Poco::FastMutex m_sendLock;
};

}
