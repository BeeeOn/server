#include <string>
#include <Poco/URI.h>

#include "di/Injectable.h"
#include "rest/RestFlow.h"
#include "restui/OAuth2CallbackRestHandler.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, OAuth2CallbackRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_END(BeeeOn, RestUI, OAuth2CallbackRestHandler)

using namespace std;
using namespace Poco;
using namespace BeeeOn::RestUI;

OAuth2CallbackRestHandler::OAuth2CallbackRestHandler():
	RestHandler("oauth2callback")
{
	registerAction<OAuth2CallbackRestHandler>("callback", &OAuth2CallbackRestHandler::callback);
}

void OAuth2CallbackRestHandler::callback(RestFlow &flow)
{
	flow.response().setStatus(200);
	flow.response()["Content-Type"] = "text/html; charset=utf-8";
	flow.response().stream() << "oauth2callback" << flush;
}
