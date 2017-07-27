#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "server/SessionManager.h"
#include "util/Base64.h"

using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, SessionManager)
BEEEON_OBJECT_REF("secureRandomProvider", &SessionManager::setSecureRandomProvider)
BEEEON_OBJECT_NUMBER("sessionExpireTime", &SessionManager::setSessionExpireTime)
BEEEON_OBJECT_NUMBER("maxUserSessions", &SessionManager::setMaxUserSessions)
BEEEON_OBJECT_END(BeeeOn, SessionManager)

SessionManager::SessionManager() :
	m_sessionCache(NULL)
{
}

SessionManager::~SessionManager()
{
	if (m_sessionCache)
		delete m_sessionCache;
}

void SessionManager::setSecureRandomProvider(SecureRandomProvider *provider)
{
	m_random = provider;
}

void SessionManager::setSessionExpireTime(const int seconds)
{
	if (seconds <= 0) {
		throw InvalidArgumentException(
			"session expire time must be greater then zero");
	}

	m_expireTime = seconds * Timespan::SECONDS;
}

void SessionManager::setMaxUserSessions(const int maxUserSessions)
{
	if (maxUserSessions <= 0) {
		throw InvalidArgumentException(
			"max user sessions must be greater then zero");
	}

	if (m_sessionCache)
		delete m_sessionCache;

	m_sessionCache = new SessionCache(maxUserSessions);
}

const ExpirableSession::Ptr SessionManager::open(
		const VerifiedIdentity &identity)
{
	if (identity.id().isNull()) {
		throw InvalidArgumentException(
				"missing verified identity ID");
	}

	if (identity.user().id().isNull()) {
		throw InvalidArgumentException(
				"missing verified identity user ID");
	}

	const User user(identity.user());

	char bSessionID[ID_LENGTH64];

	// lock here for the rest of the method
	RWLock::ScopedLock guard(m_lock, true);

	m_random->randomBytesUnlocked(bSessionID, sizeof(bSessionID));
	SessionID sessionID = Base64::encode(bSessionID, sizeof(bSessionID));

	ExpirableSession session(user.id(), sessionID, m_expireTime);
	session.setIdentityID(identity.id());

	m_sessionCache->add(session.sessionID(), session);

	if (logger().debug()) {
		logger().debug("Added session: " + session.sessionID(),
				__FILE__, __LINE__);
	}

	return m_sessionCache->get(sessionID);
}

bool SessionManager::lookup(const SessionID &id, ExpirableSession::Ptr &session)
{
	if (logger().debug())
		logger().debug("Looking up session: " + id, __FILE__, __LINE__);

	RWLock::ScopedLock guard(m_lock);

	session = m_sessionCache->get(id);

	if (session.isNull())
		return false;

	return true;
}

void SessionManager::close(const SessionID &id)
{
	if (logger().debug())
		logger().debug("Closing session: " + id, __FILE__, __LINE__);

	RWLock::ScopedLock guard(m_lock, true);
	m_sessionCache->remove(id);
}
