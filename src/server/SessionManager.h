#pragma once

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

	virtual ~SessionManager();

	virtual const Session::Ptr open(const VerifiedIdentity &identity) = 0;
	virtual bool lookup(const SessionID &id, Session::Ptr &info) = 0;
	virtual void close(const SessionID &id) = 0;
};

}
