#include <Poco/Exception.h>

#include "server/SessionManager.h"
#include "util/Base64.h"

using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT(SessionManager, BeeeOn::SessionManager)

const SessionID SessionManager::open(const User &user)
{
	TRACE_METHOD();

	if (user.id().isNull()) {
		if (m_logger.debug()) {
			m_logger.debug("Missing user ID for: " + user.email(),
					__FILE__, __LINE__);
		}

		throw InvalidArgumentException("missing user ID");
	}

	Timespan timespan(m_expireTime, 0);
	char bSessionID[ID_LENGTH64];

	// lock here for the rest of the method
	RWLock::ScopedLock guard(m_lock, true);

	m_random->randomBytesUnlocked(bSessionID, sizeof(bSessionID));
	SessionID sessionID = Base64::encode(bSessionID, sizeof(bSessionID));

	ExpirableSession session(user.id(), sessionID, timespan);

	char b[ID_LENGTH64];
	m_random->randomBytesUnlocked(b, sizeof(b));
	SessionID id = Base64::encode(b, sizeof(b));
	m_sessionCache->add(session.sessionID(), session);

	if (m_logger.debug()) {
		m_logger.debug("Added session: " + session.sessionID(),
				__FILE__, __LINE__);
	}

	return session.sessionID();
}

bool SessionManager::lookup(const SessionID &id, SessionPtr &session)
{
	TRACE_METHOD();

	if (m_logger.debug())
		m_logger.debug("Looking up session: " + id, __FILE__, __LINE__);

	RWLock::ScopedLock guard(m_lock);

	session = m_sessionCache->get(id);

	if (session.isNull())
		return false;

	return true;
}

void SessionManager::close(const SessionID &id)
{
	TRACE_METHOD();

	if (m_logger.debug())
		m_logger.debug("Closing session: " + id, __FILE__, __LINE__);

	RWLock::ScopedLock guard(m_lock, true);
	m_sessionCache->remove(id);
}
