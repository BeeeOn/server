#ifndef BEEEON_SESSION_CACHE_H
#define BEEEON_SESSION_CACHE_H

#include <map>

#include <Poco/AbstractCache.h>
#include <Poco/UniqueExpireStrategy.h>

#include "model/User.h"
#include "server/Session.h"
#include "util/Loggable.h"

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
	Session,
	Poco::UniqueExpireStrategy<SessionID, Session>>,
	public Loggable
{
public:
	SessionCache(unsigned int maxUserSessions);
	~SessionCache();

	/**
	 * Adds new session with given sessionID to session table and creates
	 * new reverse record to map existing user's session.
	 */
	void add(const SessionID &sessionID, Session &session);

	/**
	 * Removes session from session and user's session tables.
	 */
	void remove(const SessionID &sessionID);

	/**
	 * Removes session record from set in reverse user session map
	 */
	void removeSession(const SessionID &sessionID, const UserID &userID);

private:
	std::map<UserID, std::set<SessionID>> m_userSessionSet;
	unsigned int m_userSessions;
};

}

#endif
