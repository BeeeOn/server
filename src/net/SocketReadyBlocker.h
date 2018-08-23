#pragma once

#include <string>
#include <vector>

#include <Poco/Clock.h>
#include <Poco/Event.h>
#include <Poco/Nullable.h>
#include <Poco/Timespan.h>

#include <Poco/Net/StreamSocket.h>

#include "loop/StoppableLoop.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief Implements a fake loop that waits until a remote system
 * starts listening on the configured socket. It serves as a barrier
 * while starting loops that depend on each other somehow.
 */
class SocketReadyBlocker : public StoppableLoop, Loggable {
public:
	SocketReadyBlocker();

	/**
	 * @brief Set host to connect until up.
	 */
	void setHost(const std::string &host);

	/**
	 * @brief Set port to connect until up.
	 */
	void setPort(int port);

	/**
	 * @brief Configure timeout of the blocking operation. The timeout
	 * can be negative (infinity). If the timeout is exceeded, the SocketReadyBlocker
	 * fails to start with TimeoutException.
	 */
	void setTimeout(const Poco::Timespan &timeout);

	/**
	 * @brief Configure interval between two consecutive connections
	 * to the remote system (to not DoS it).
	 */
	void setInterval(const Poco::Timespan &interval);

	/**
	 * @brief Set payload to send to the remote server in case when the TCP connection
	 * successfully opens.
	 *
	 * Format: a list of bytes is expected in format "HH", "HH", ...
	 */
	void setRequest(const std::list<std::string> &payload);

	/**
	 * @brief Set expected response header from the remote server. If set,
	 * the response must contain at least the header at its beginning.
	 *
	 * Format: a list of bytes is expected in format "HH", "HH", ...
	 */
	void setResponseHeader(const std::list<std::string> &payload);

	/**
	 * @brief Set expected response body from the remote server. If set,
	 * the response must contain the header (if set) and just after the
	 * header, the rest of the response payload must match the body.
	 *
	 * Format: a list of bytes is expected in format "HH", "HH", ...
	 */
	void setResponseBody(const std::list<std::string> &payload);

	/**
	 * @brief Set count of bytes to expect to receive. If negative,
	 * the response size is not considered.
	 */
	void setResponseSize(int size);

	/**
	 * @brief Start connecting to the remote system until it is up or a timeout
	 * exceeds. The method intentionally blocks.
	 *
	 * @throws Poco::TimeoutException when timeout exceeds
	 */
	void start() override;

	/**
	 * @brief Interrupts the periodic connections to the remote system without
	 * any error.
	 */
	void stop() override;

protected:
	bool ping() const;
	void sendRequest(Poco::Net::StreamSocket &socket) const;
	void checkResponse(Poco::Net::StreamSocket &socket) const;
	void checkTimeout(const Poco::Clock &clock) const;
	std::string remote() const;
	static std::vector<uint8_t> parsePayload(const std::list<std::string> &payload);
	static std::string payloadToString(const std::vector<uint8_t> &payload);

private:
	std::string m_host;
	int m_port;
	Poco::Timespan m_timeout;
	Poco::Timespan m_interval;
	std::vector<uint8_t> m_request;
	std::vector<uint8_t> m_responseHeader;
	std::vector<uint8_t> m_responseBody;
	Poco::Nullable<size_t> m_responseSize;
	Poco::Event m_event;
};

}
