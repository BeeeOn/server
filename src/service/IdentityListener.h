#pragma once

#include <Poco/SharedPtr.h>

#include "model/Gateway.h"
#include "model/Identity.h"
#include "model/User.h"
#include "model/VerifiedIdentity.h"
#include "server/AccessLevel.h"

namespace BeeeOn {

class IdentityInviteEvent {
public:
	IdentityInviteEvent();

	void setOriginator(const User &originator);
	const User &originator() const;

	void setGateway(const Gateway &gateway);
	const Gateway &gateway() const;

	void setIdentity(const Identity &identity);
	const Identity &identity() const;

	void setLevel(const AccessLevel &level);
	const AccessLevel &level() const;

private:
	User m_originator;
	Gateway m_gateway;
	Identity m_identity;
	AccessLevel m_level;
};

class VerifiedIdentityEvent {
public:
	VerifiedIdentityEvent();

	void setVerifiedIdentity(const VerifiedIdentity &identity);
	const VerifiedIdentity &verifiedIdentity() const;

private:
	VerifiedIdentity m_identity;
};

/**
 * @brief IdentityListener is an interface providing access to events related
 * to identities.
 */
class IdentityListener {
public:
	typedef Poco::SharedPtr<IdentityListener> Ptr;

	virtual ~IdentityListener();

	/**
	 * @brief Fired when an identity logins for the first time.
	 * At that moment, we already know its identity provider and
	 * thus such identity is always verified.
	 */
	virtual void onFirstLogin(const VerifiedIdentityEvent &e);

	/**
	 * @brief Fired when an identity is invited to use a certain
	 * gateway.
	 */
	virtual void onInvite(const IdentityInviteEvent &e);
};

}
