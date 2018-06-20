#pragma once

#include <string>

#include <Poco/Clock.h>
#include <Poco/Event.h>
#include <Poco/Timespan.h>

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
	void checkTimeout(const Poco::Clock &clock) const;
	std::string remote() const;

private:
	std::string m_host;
	int m_port;
	Poco::Timespan m_timeout;
	Poco::Timespan m_interval;
	Poco::Event m_event;
};

}
