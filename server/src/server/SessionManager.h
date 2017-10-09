#ifndef BEEEON_SESSION_MANAGER_H
#define BEEEON_SESSION_MANAGER_H

#include <Poco/SharedPtr.h>

#include "model/VerifiedIdentity.h"
#include "server/Session.h"

namespace BeeeOn {

/**
 * A user session management.
 */
class SessionManager {
public:
	typedef Poco::SharedPtr<SessionManager> Ptr;

	~SessionManager();

	virtual const ExpirableSession::Ptr open(const VerifiedIdentity &identity) = 0;
	virtual bool lookup(const SessionID &id, ExpirableSession::Ptr &info) = 0;
	virtual void close(const SessionID &id) = 0;
};

}

#endif
