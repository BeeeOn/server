#ifndef BEEEON_SESSION_H
#define BEEEON_SESSION_H

#include <Poco/Timespan.h>
#include <Poco/SharedPtr.h>

#include "model/User.h"
#include "model/VerifiedIdentity.h"

namespace BeeeOn {

typedef std::string SessionID;

/**
 * Base class to hold session data (userID, sessionID).
 */
class Session
{
public:
	Session(const UserID &userID, const SessionID &sessionID);

	void setIdentityID(const VerifiedIdentityID &id);
	const VerifiedIdentityID &identityID() const;

	void setUserId(const UserID &id);
	const UserID userID() const;

	const SessionID sessionID() const;

private:
	VerifiedIdentityID m_identityID;
	UserID m_userID;
	SessionID m_sessionID;
};

/**
 * Enables expirable session feature using Poco
 */
class ExpirableSession : public Session
{
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
