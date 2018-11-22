#pragma once

#include <Poco/Clock.h>
#include <Poco/Mutex.h>
#include <Poco/Timespan.h>
#include <Poco/SharedPtr.h>

#include "gwmessage/GWMessage.h"
#include "model/GatewayID.h"

namespace BeeeOn {

/**
 * @brief GatewayRateLimiter controls the message rate per gateway.
 * It is supposed to slow down communication in case of high traffic
 * load from gateways or to mitigate some class of DoS-kind attacks.
 */
class GatewayRateLimiter {
public:
	typedef Poco::SharedPtr<GatewayRateLimiter> Ptr;

	GatewayRateLimiter(const GatewayID &id);
	virtual ~GatewayRateLimiter();

	const GatewayID &gatewayID() const;

	/**
	 * @returns true if the given message is allowed
	 * to be processed. Otherwise, it should be discarded.
	 */
	virtual bool accept(const GWMessage::Ptr msg);

	/**
	 * @returns true if the incoming message is allowed to
	 * be processed. In this case, we consider any message
	 * without knowing its contents. We can thus limit also
	 * non-application messages.
	 */
	virtual bool accept() = 0;

private:
	const GatewayID m_id;
};

class GatewayRateLimiterFactory {
public:
	typedef Poco::SharedPtr<GatewayRateLimiterFactory> Ptr;

	virtual ~GatewayRateLimiterFactory();

	/**
	 * Create a new GatewayRateLimiterFactory instance for the
	 * given gateway ID.
	 */
	virtual GatewayRateLimiter::Ptr create(const GatewayID &id) = 0;
};

/**
 * @brief GatewayRateLimiterCache holds GatewayRateLimiter instances
 * for a certain amount of time. This allows to keep track of rate limiting
 * for gateways that disconnect and returns back in a minute.
 */
class GatewayRateLimiterCache {
public:
	GatewayRateLimiterCache();

	void setKeepTimeout(const Poco::Timespan &timeout);

	/**
	 * @brief Keep the given GatewayRateLimiter instance until
	 * its timeout expires. If there is already a rateLimiter
	 * for that gateway, it is overriden.
	 */
	void keep(GatewayRateLimiter::Ptr rateLimiter);

	/**
	 * @brief Find a GatewayRateLimiter instance for the given
	 * gateway ID and drop it out of the cache.
	 *
	 * @returns appropriate instance of GatewayRateLimiter or nullptr.
	 */
	GatewayRateLimiter::Ptr findAndDrop(const GatewayID &id);

protected:
	void eraseExpired(const Poco::Clock &now = {});

private:
	Poco::Timespan m_keepTimeout;

	typedef std::multimap<Poco::Clock, GatewayRateLimiter::Ptr> ExpireQueue;
	typedef std::map<GatewayID, ExpireQueue::const_iterator> RecordMap;

	RecordMap m_records;
	ExpireQueue m_expireQueue;
	Poco::FastMutex m_lock;
};

}
