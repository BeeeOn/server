#include <Poco/Exception.h>

#include "server/SessionCache.h"
#include "Debug.h"

using namespace Poco;
using namespace BeeeOn;

SessionCache::SessionCache(unsigned int maxUserSessions):
	m_logger(LOGGER_CLASS(this))
{
	m_userSessions = maxUserSessions;
}

SessionCache::~SessionCache()
{
	m_userSessionSet.clear();
}

/**
 * Adds new session with given sessionID to session table and creates
 * new reverse record to map existing user's session.
 */
void SessionCache::add(const SessionID &sessionID, ExpirableSession &session)
{
	UserID userID = session.userID();
	auto userSession = m_userSessionSet.find(userID);

	if (userSession == m_userSessionSet.end()) {
		if (m_logger.debug()) {
			m_logger.debug(
				"user: " + userID.toString() +
				" new session: " + sessionID,
				__FILE__, __LINE__);
		}

		std::set<SessionID> sessionSet;
		sessionSet.emplace(sessionID);
		m_userSessionSet.emplace(std::make_pair(userID, sessionSet));
	} else {
		if (m_logger.debug()) {
			m_logger.debug(
				"user: " + userID.toString() +
				" new session: " + sessionID +
				" sessions: " +
				std::to_string(userSession->second.size()),
				__FILE__, __LINE__);
		}

		if (userSession->second.size() >= m_userSessions)
			throw IllegalStateException("Max session size reached");

		userSession->second.emplace(sessionID);
	}

	doAdd(sessionID, session);
}

/**
 * Removes session from session and user's session tables.
 */
void SessionCache::remove(const SessionID &sessionID)
{
	auto it = _data.find(sessionID);

	if (it != _data.end()) {
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
void SessionCache::removeSession(const SessionID &sessionID, const UserID &userID)
{
	auto itUser = m_userSessionSet.find(userID);

	if (itUser != m_userSessionSet.end()) {
		auto itSession = itUser->second.find(sessionID);

		if (itSession != itUser->second.end())
			itUser->second.erase(itSession);

		if (itUser->second.size() == 0)
			m_userSessionSet.erase(itUser);

		if (m_logger.debug()) {
			m_logger.debug("user: " + userID.toString() +
					" remove session, sessions: " +
					std::to_string(itUser->second.size()),
					__FILE__, __LINE__);
		}
	}
}

