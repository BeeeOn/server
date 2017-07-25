#ifndef BEEEON_SESSION_MANAGER_H
#define BEEEON_SESSION_MANAGER_H

#include <Poco/RWLock.h>
#include <Poco/Logger.h>

#include "provider/RandomProvider.h"
#include "model/VerifiedIdentity.h"
#include "server/SessionCache.h"
#include "server/Session.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * A user session management.
 */
class SessionManager : public Loggable {
public:
	enum {
		ID_LENGTH64 = 64, /**< 512 bits long session ID */
	};

	SessionManager();
	~SessionManager();

	void setSecureRandomProvider(SecureRandomProvider *provider);
	void setSessionExpireTime(const int sessionExpireTime);
	void setMaxUserSessions(const int maxUserSessions);

	/**
	 * Open a new session for the given info.
	 * If there is a session with equivalent identification
	 * info, an exception is thrown.
	 * Returns a new session ID.
	 */
	const ExpirableSession::Ptr open(const VerifiedIdentity &identity);

	/**
	 * Lookup a session information by ID.
	 * Returns false if there is no such session.
	 */
	bool lookup(const SessionID &id, ExpirableSession::Ptr &info);

	/**
	 * Close session of the given ID.
	 * If there is no such session, it returns silently.
	 */
	void close(const SessionID &id);

private:
	Poco::RWLock m_lock;
	SecureRandomProvider *m_random;
	SessionCache *m_sessionCache;
	unsigned int m_expireTime;
};

}

#endif
