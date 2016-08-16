#ifndef BEEEON_NULL_SESSION_MANAGER_H
#define BEEEON_NULL_SESSION_MANAGER_H
#include <map>
#include <Poco/RWLock.h>
#include <Poco/Timespan.h>

#include "di/InjectorTarget.h"
#include "provider/RandomProvider.h"
#include "model/User.h"

namespace BeeeOn {

typedef std::string SessionID;
typedef std::string Email;

/**
 * Base class to hold session data (userID, sessionID).
 * Email is only for future use. Currently useless.
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

/**
 * A user session management.
 */
class SessionManager : public AbstractInjectorTarget {
public:
	typedef std::map<SessionID, User> Table;
	typedef std::map<std::string, SessionID> IDTable;

	enum {
		ID_LENGTH64 = 64, /**< 512 bits long session ID */
	};

	SessionManager()
	{
		injector<SessionManager, SecureRandomProvider>(
			"secureRandomProvider",
			&SessionManager::setSecureRandomProvider);
	}

	void setSecureRandomProvider(SecureRandomProvider *provider)
	{
		m_random = provider;
	}

	/**
	 * Open a new session for the given info.
	 * If there is a session with equivalent identification
	 * info, an exception is thrown.
	 * Returns a new session ID.
	 */
	const SessionID open(const User &user);

	/**
	 * Lookup a session information by ID.
	 * Returns false if there is no such session.
	 */
	bool lookup(const SessionID &id, User &user);

	/**
	 * Close session of the given ID.
	 * If there is no such session, it returns silently.
	 */
	void close(const SessionID &id);

private:
	/**
	 * Table: session ID -> Info.
	 */
	Table m_table;
	/**
	 * Reverse table: e-mail -> session ID.
	 */
	IDTable m_idTable;
	Poco::RWLock m_lock;
	SecureRandomProvider *m_random;
};

}

#endif
