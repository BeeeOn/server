#ifndef BEEEON_SESSION_CACHE_H
#define BEEEON_SESSION_CACHE_H

#include <map>

#include <Poco/AbstractCache.h>
#include <Poco/UniqueExpireStrategy.h>

#include "model/User.h"
#include "server/Session.h"

namespace BeeeOn {

/*
 * SessionCache holds both session and its reverse records for each session
 * in relation with its user (by their UserID).
 *
 * Limits each user to a number of sessions to prevent DoS attacks.
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

}

#endif
