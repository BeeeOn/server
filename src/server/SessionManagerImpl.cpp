#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "server/SessionManagerImpl.h"
#include "util/Base64.h"

using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, SessionManagerImpl)
BEEEON_OBJECT_CASTABLE(SessionManager)
BEEEON_OBJECT_REF("secureRandomProvider", &SessionManagerImpl::setSecureRandomProvider)
BEEEON_OBJECT_TIME("sessionExpireTime", &SessionManagerImpl::setSessionExpireTime)
BEEEON_OBJECT_NUMBER("maxUserSessions", &SessionManagerImpl::setMaxUserSessions)
BEEEON_OBJECT_END(BeeeOn, SessionManagerImpl)

SessionManagerImpl::SessionManagerImpl()
{
}

void SessionManagerImpl::setSecureRandomProvider(SecureRandomProvider *provider)
{
	m_random = provider;
}

void SessionManagerImpl::setSessionExpireTime(const Timespan &time)
{
	if (time <= 0) {
		throw InvalidArgumentException(
			"session expire time must be greater then zero");
	}

	m_expireTime = time;
}

void SessionManagerImpl::setMaxUserSessions(const int maxUserSessions)
{
	if (maxUserSessions <= 0) {
		throw InvalidArgumentException(
			"max user sessions must be greater then zero");
	}

	m_sessionCache = new SessionCache(maxUserSessions);
}

const Session::Ptr SessionManagerImpl::open(
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

	Session session(sessionID);
	session.setUserID(user.id());
	session.setExpiration(Timestamp{} + m_expireTime);
	session.setIdentityID(identity.id());
	session.setLocale(user.locale());

	m_sessionCache->add(session.sessionID(), session);

	if (logger().debug()) {
		logger().debug("Added session: " + session.sessionID(),
				__FILE__, __LINE__);
	}

	return m_sessionCache->get(sessionID);
}

bool SessionManagerImpl::lookup(const SessionID &id, Session::Ptr &session)
{
	if (logger().debug())
		logger().debug("Looking up session: " + id, __FILE__, __LINE__);

	RWLock::ScopedLock guard(m_lock);

	session = m_sessionCache->get(id);

	if (session.isNull())
		return false;

	return true;
}

void SessionManagerImpl::close(const SessionID &id)
{
	if (logger().debug())
		logger().debug("Closing session: " + id, __FILE__, __LINE__);

	RWLock::ScopedLock guard(m_lock, true);
	m_sessionCache->remove(id);
}
