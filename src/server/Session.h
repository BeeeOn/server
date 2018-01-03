#ifndef BEEEON_SESSION_H
#define BEEEON_SESSION_H

#include <Poco/Timespan.h>
#include <Poco/SharedPtr.h>

#include "l10n/Locale.h"
#include "model/User.h"
#include "model/VerifiedIdentity.h"

namespace BeeeOn {

typedef std::string SessionID;

/**
 * Base class to hold session data (userID, sessionID).
 */
class Session {
public:
	typedef Poco::SharedPtr<Session> Ptr;

	Session(const UserID &userID, const SessionID &sessionID);
	virtual ~Session();

	void setIdentityID(const VerifiedIdentityID &id);
	const VerifiedIdentityID &identityID() const;

	void setUserId(const UserID &id);
	const UserID userID() const;

	void setLocale(const Locale &locale);
	const Locale &locale() const;

	const SessionID sessionID() const;

private:
	VerifiedIdentityID m_identityID;
	UserID m_userID;
	SessionID m_sessionID;
	Locale m_locale;
};

/**
 * Enables expirable session feature using Poco
 */
class ExpirableSession : public Session {
public:
	typedef Poco::SharedPtr<ExpirableSession> Ptr;

	ExpirableSession(
		const UserID &userID,
		const SessionID &sessionID,
		Poco::Timespan &timespan);

	const Poco::Timestamp& getExpiration() const;

private:
	Poco::Timestamp m_tstamp;
};

}

#endif
