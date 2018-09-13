#pragma once

#include "provider/AuthProvider.h"
#include "rest/JSONRestHandler.h"
#include "server/Session.h"

namespace BeeeOn {

class AuthService;

class AuthRestHandler : public JSONRestHandler {
public:
	AuthRestHandler();

	void setAuthService(AuthService *service);

	void list(RestFlow &flow);
	void login(RestFlow &flow);
	void logout(RestFlow &flow);

protected:
	Session::Ptr doLogin(const Credentials &credentials);

private:
	AuthService *m_authService;
};

}
