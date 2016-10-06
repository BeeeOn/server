#include <Poco/Exception.h>

#include "server/SessionManager.h"
#include "util/Base64.h"

using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT(SessionManager, BeeeOn::SessionManager)

const ExpirableSession::Ptr SessionManager::open(
		const VerifiedIdentity &identity)
{
	TRACE_METHOD();

	if (identity.id().isNull()) {
		throw InvalidArgumentException(
				"missing verified identity ID");
	}

	if (identity.user().id().isNull()) {
		throw InvalidArgumentException(
				"missing verified identity user ID");
	}

	const User user(identity.user());

	Timespan timespan(m_expireTime, 0);
	char bSessionID[ID_LENGTH64];

	// lock here for the rest of the method
	RWLock::ScopedLock guard(m_lock, true);

	m_random->randomBytesUnlocked(bSessionID, sizeof(bSessionID));
	SessionID sessionID = Base64::encode(bSessionID, sizeof(bSessionID));

	ExpirableSession session(user.id(), sessionID, timespan);
	session.setIdentityID(identity.id());

	m_sessionCache->add(session.sessionID(), session);

	if (m_logger.debug()) {
		m_logger.debug("Added session: " + session.sessionID(),
				__FILE__, __LINE__);
	}

	return m_sessionCache->get(sessionID);
}

bool SessionManager::lookup(const SessionID &id, ExpirableSession::Ptr &session)
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
