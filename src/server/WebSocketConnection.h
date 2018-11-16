#pragma once

#include <functional>

#include <Poco/AutoPtr.h>
#include <Poco/Buffer.h>
#include <Poco/Mutex.h>
#include <Poco/NObserver.h>
#include <Poco/RefCountedObject.h>

#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/WebSocket.h>

#include "io/IOStats.h"
#include "util/Loggable.h"

namespace BeeeOn {

class WebSocketConnection :
	public Poco::RefCountedObject,
	protected Loggable {
public:
	typedef Poco::AutoPtr<WebSocketConnection> Ptr;
	typedef std::function<void(WebSocketConnection::Ptr)> EnqueueReadable;

	WebSocketConnection(
		const std::string &id,
		const Poco::Net::WebSocket &webSocket,
		Poco::Net::SocketReactor &reactor,
		const EnqueueReadable &enqueueReadable,
		const size_t maxFrameSize);

	virtual ~WebSocketConnection();

	void addToReactor() const;
	void removeFromReactor() const;

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

	/**
	 * @brief The Callback method invoked from the reactor thread.
	 *
	 * This method is intended just to determine there are data on the socket.
	 * The data reading takes place in the thread pool of the GatewayCommunicator.
	 */
	void onReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification> &notification);

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
	Poco::Net::SocketReactor &m_reactor;
	EnqueueReadable m_enqueueReadable;
	size_t m_maxFrameSize;
	Poco::FastMutex m_sendLock;
	Poco::NObserver<WebSocketConnection, Poco::Net::ReadableNotification> m_readableObserver;
};

}
