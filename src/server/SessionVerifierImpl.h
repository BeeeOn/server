#pragma once

#include "server/SessionManager.h"
#include "server/SessionVerifier.h"

namespace BeeeOn {

class SessionVerifierImpl : public SessionVerifier {
public:
	void setSessionManager(SessionManager *manager)
	{
		m_sessionManager = manager;
	}

protected:
	Session::Ptr doVerifyAuthorized(
			const std::string &scheme,
			const std::string &authInfo) override;

protected:
	SessionManager *m_sessionManager;
};

}
