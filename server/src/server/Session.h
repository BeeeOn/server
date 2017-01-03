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
	Session(const UserID &userID, const SessionID &sessionID) :
		m_userID(userID),
		m_sessionID(sessionID)
	{
	}

	void setIdentityID(const VerifiedIdentityID &id)
	{
		m_identityID = id;
	}

	const VerifiedIdentityID &identityID() const
	{
		return m_identityID;
	}

	void setUserId(const UserID &id)
	{
		m_userID = id;
	}

	const UserID userID() const
	{
		return m_userID;
	}

	const SessionID sessionID() const
	{
		return m_sessionID;
	}

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

	ExpirableSession(const UserID &userID, const SessionID &sessionID,
			Poco::Timespan &timespan) :
		Session(userID, sessionID),
		m_tstamp()
	{
		// Set expiration time
		m_tstamp += timespan;
	}

	const Poco::Timestamp& getExpiration() const
	{
		return m_tstamp;
	}

private:
	Poco::Timestamp m_tstamp;
};

}

#endif