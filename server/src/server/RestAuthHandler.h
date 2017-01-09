#ifndef BEEEON_REST_AUTH_HANDLER_H
#define BEEEON_REST_AUTH_HANDLER_H

#include <string>
#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "server/RestHandler.h"
#include "server/Session.h"
#include "service/AuthService.h"

namespace BeeeOn {

class RestAuthHandler : public RestHandler, public AbstractInjectorTarget {
public:
	RestAuthHandler();

	void extractAuthData(std::istream &in,
			std::string &provider,
			std::string &authCode);

	template <typename Request, typename Response>
	bool echoSessionIfExists(Request &request, Response &response)
	{
		if (!request.hasCredentials())
			return false;

		std::string scheme;
		SessionID session;

		request.getCredentials(scheme, session);
		sendResult(response, session);
		return true;
	}

	template <typename Context>
	void handleLogin(Context &context)
	{
		if (echoSessionIfExists(context.request(),
					context.response()))
			return;

		std::istream &in = context.request().stream();

		try {
			sendResult(
				context.response(),
				handleLogin(in)
			);
		}
		catch (const Poco::Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			sendUnauthorized(context.response());
		}
	}

	SessionID handleLogin(std::istream &in);

	template <typename Context>
	void handleLogout(Context &context)
	{
		std::string scheme;
		SessionID session;

		try {
			context.request().getCredentials(scheme, session);
		} catch(const Poco::Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			sendUnauthorized(context.response());
		}

		m_authService->logout(session);
		sendSuccess(context.response());
	}

	void setAuthService(AuthService *service)
	{
		m_authService = service;
	}

private:
	AuthService *m_authService;
};

}

#endif
