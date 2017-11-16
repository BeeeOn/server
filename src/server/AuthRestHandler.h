#ifndef BEEEON_AUTH_REST_HANDLER_H
#define BEEEON_AUTH_REST_HANDLER_H

#include "rest/JSONRestHandler.h"

namespace BeeeOn {

class AuthService;

class AuthRestHandler : public JSONRestHandler {
public:
	AuthRestHandler();

	void setAuthService(AuthService *service);

	void list(RestFlow &flow);
	void login(RestFlow &flow);
	void logout(RestFlow &flow);

private:
	AuthService *m_authService;
};

}

#endif
