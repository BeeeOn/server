#ifndef BEEEON_NULL_SESSION_MANAGER_H
#define BEEEON_NULL_SESSION_MANAGER_H

#include <map>

#include <Poco/RWLock.h>
#include <Poco/AbstractCache.h>
#include <Poco/UniqueExpireStrategy.h>
#include <Poco/ExpirationDecorator.h>
#include <Poco/Timespan.h>
#include <Poco/SharedPtr.h>
#include <Poco/Logger.h>

#include "di/InjectorTarget.h"
#include "provider/RandomProvider.h"
#include "model/User.h"
#include "Debug.h"

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

/*
 * SessionCache holds both session and its reverse records for each session
 * in relation with its user (by their UserID).
 *
 * Extends Poco's UniqueExpireCache.
 *
 * Limits each user to have MAX_USER_SESSIONS to prevent DoS attacks.
 *
 * This class is NOT thread-safe
 */
class SessionCache : public Poco::AbstractCache<SessionID,
	ExpirableSession,
	Poco::UniqueExpireStrategy<SessionID, ExpirableSession>>
{
public:
	SessionCache(unsigned int maxUserSessions)
	{
		m_userSessions = maxUserSessions;
	}

	~SessionCache()
	{
		m_userSessionSet.clear();
	}

	/**
	 * Adds new session with given sessionID to session table and creates
	 * new reverse record to map existing user's session.
	 */
	void add(const SessionID &sessionID, ExpirableSession &session)
	{
		UserID userID = session.userID();
		auto userSession = m_userSessionSet.find(userID);

		if (userSession == m_userSessionSet.end()) {
			std::set<SessionID> sessionSet;
			sessionSet.emplace(sessionID);
			m_userSessionSet.emplace(std::make_pair(userID, sessionSet));
		} else {
			if (userSession->second.size() > m_userSessions)
				throw Poco::IllegalStateException("Max session size reached");

			userSession->second.emplace(sessionID);
		}

		doAdd(sessionID, session);
	}

	/**
	 * Removes session from session and user's session tables.
	 */
	void remove(const SessionID &sessionID)
	{
		auto it = _data.find(sessionID);

		if (it != _data.end())
		{
			UserID userID = it->second->userID();

			// Remove session from user map
			removeSession(sessionID, userID);

			// Remove session from session cache
			doRemove(it);
		}
	}

	/**
	 * Removes session record from set in reverse user session map
	 */
	void removeSession(const SessionID &sessionID, const UserID &userID)
	{
		auto itUser = m_userSessionSet.find(userID);

		if (itUser != m_userSessionSet.end()) {
			auto itSession = itUser->second.find(sessionID);

			if (itSession != itUser->second.end())
				itUser->second.erase(itSession);

			if (itUser->second.size() == 0)
				m_userSessionSet.erase(itUser);
		}
	}

private:
	std::map<UserID, std::set<SessionID>> m_userSessionSet;
	unsigned int m_userSessions;
};

/**
 * A user session management.
 */
class SessionManager : public AbstractInjectorTarget
{
public:
	typedef Poco::SharedPtr<ExpirableSession> SessionPtr;

	enum {
		ID_LENGTH64 = 64, /**< 512 bits long session ID */
	};

	SessionManager() :
		m_logger(LOGGER_CLASS(this))
	{
		injector<SessionManager, SecureRandomProvider>(
			"secureRandomProvider",
			&SessionManager::setSecureRandomProvider);
		numberInjector("sessionExpireTime",
				(NumberSetter) &SessionManager::setSessionExpireTime);
		numberInjector("maxUserSessions",
				(NumberSetter) &SessionManager::setMaxUserSessions);
	}

	~SessionManager()
	{
		delete m_sessionCache;
	}

	void setSecureRandomProvider(SecureRandomProvider *provider)
	{
		m_random = provider;
	}

	void setSessionExpireTime(const int sessionExpireTime)
	{
		m_expireTime = sessionExpireTime;
	}

	void setMaxUserSessions(const int maxUserSessions)
	{
		m_sessionCache = new SessionCache(maxUserSessions);
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
	bool lookup(const SessionID &id, SessionPtr &info);

	/**
	 * Close session of the given ID.
	 * If there is no such session, it returns silently.
	 */
	void close(const SessionID &id);

private:
	Poco::RWLock m_lock;
	Poco::Logger &m_logger;
	SecureRandomProvider *m_random;
	SessionCache *m_sessionCache;
	unsigned int m_expireTime;
};

}

#endif
