#pragma once

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

}
