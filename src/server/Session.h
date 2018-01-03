#ifndef BEEEON_SESSION_H
#define BEEEON_SESSION_H

#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>
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

	Session(const UserID &userID, const SessionID &sessionID,
			const Poco::Timespan &expireIn);
	virtual ~Session();

	void setIdentityID(const VerifiedIdentityID &id);
	const VerifiedIdentityID &identityID() const;

	void setUserID(const UserID &id);
	const UserID userID() const;

	void setLocale(const Locale &locale);
	const Locale &locale() const;

	const SessionID sessionID() const;

	void setExpiration(const Poco::Timestamp &expiration);
	const Poco::Timestamp& getExpiration() const;

private:
	VerifiedIdentityID m_identityID;
	UserID m_userID;
	SessionID m_sessionID;
	Locale m_locale;
	Poco::Timestamp m_expiration;
};

}

#endif
