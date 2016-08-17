#ifndef BEEEON_UI_AUTH_H
#define BEEEON_UI_AUTH_H

#include "ui/Handler.h"

namespace Poco {
	class Logger;
}

namespace BeeeOn
{

class Auth : public Handler
{
public:
	static void handlePost(UIRouteContext &context);
	static void handleDelete(UIRouteContext &context);

private:
	static void handlePost(Poco::Logger &logger,
			UIRequest &request,
			UIResponse &response,
			AuthService &authService);
	static void extractAuthData(
			Poco::Logger &logger,
			std::istream &data,
			std::string &provider,
			std::string &authCode);
	static bool echoSessionIfExists(
			UIRequest &request,
			UIResponse &response);
};
}
#endif /* BEEEON_UI_AUTH_H */
