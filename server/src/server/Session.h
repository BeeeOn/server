#ifndef BEEEON_SESSION_H
#define BEEEON_SESSION_H

#include <Poco/Timespan.h>

namespace BeeeOn {

typedef std::string SessionID;
typedef std::string Email;

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

	const Email email() const
	{
		return m_email;
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
	Email m_email;
	UserID m_userID;
	SessionID m_sessionID;
};

/**
 * Enables expirable session feature using Poco
 */
class ExpirableSession : public Session
{
public:
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
