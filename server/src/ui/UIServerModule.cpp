#include <map>
#include <Poco/StreamCopier.h>
#include <Poco/Net/NetException.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include "di/Injectable.h"
#include "service/AuthService.h"
#include "server/Route.h"
#include "server/Session.h"
#include "ui/UIServerModule.h"
#include "Debug.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace BeeeOn;

void UIServerModule::installRoutes()
{
	m_factory->noRoute([](UIRouteContext &context) {
		RestHandler::sendNoRoute(context.response());
	});
	m_factory->noOperation([](UIRouteContext &context) {
		RestHandler::sendNoOperation(context.response());
	});
	m_factory->POST("/auth", [&](UIRouteContext &context) {
		m_authHandler->handleLogin(context);
	}, false);
	m_factory->DELETE("/auth", [&](UIRouteContext &context) {
		m_authHandler->handleLogout(context);
	});
}

BEEEON_OBJECT_BEGIN(BeeeOn, UIServerModule)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_REF("sessionVerifier", &UIServerModule::setSessionVerifier)
BEEEON_OBJECT_REF("userService", &UIServerModule::setUserService)
BEEEON_OBJECT_REF("authHandler", &UIServerModule::setAuthHandler)
BEEEON_OBJECT_REF("deviceService", &UIServerModule::setDeviceService)
BEEEON_OBJECT_NUMBER("port", &UIServerModule::setPort)
BEEEON_OBJECT_HOOK("done", &UIServerModule::installRoutes)
BEEEON_OBJECT_END(BeeeOn, UIServerModule)
