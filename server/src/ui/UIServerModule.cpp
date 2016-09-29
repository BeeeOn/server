#include <map>
#include <Poco/StreamCopier.h>
#include <Poco/Net/NetException.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include "service/AuthService.h"
#include "server/Route.h"
#include "model/JSONSerializer.h"
#include "ui/UIServerModule.h"
#include "Debug.h"

#include "ui/Auth.h"
#include "ui/PlaceHandler.h"
#include "ui/GatewayHandler.h"
#include "ui/LocationHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace BeeeOn;

void handleNoRoute(UIRouteContext &context)
{
	TRACE_FUNC();

	context.response().setStatus(UIResponse::HTTP_NOT_FOUND);
	context.response().setReason("Not found");
}

static void handleNoOperation(UIRouteContext &context)
{
	TRACE_FUNC();

	context.response().setStatus(UIResponse::HTTP_BAD_REQUEST);
	context.response().setReason("Bad request");
}

static void verifyAuthorized(const UIRequest &request,
		const UIRoute &route, UIServerModule &module)
{
	TRACE_FUNC();

	if (request.hasCredentials()) {
		string scheme;
		string authInfo;
		Poco::SharedPtr<ExpirableSession> session;

		request.getCredentials(scheme, authInfo);

		if (!module.sessionManager().lookup(authInfo, session))
			throw NotAuthenticatedException("Session not found");

		return;
	}

	throw NotAuthenticatedException("credentials not found");
}

namespace BeeeOn {

void factorySetup(UIServerRequestHandlerFactory &factory)
{
	factory.noRoute(handleNoRoute);
	factory.noOperation(handleNoOperation);
	factory.sessionVerifier(verifyAuthorized);
	factory.POST("/auth", Auth::handlePost, false);
	factory.DELETE("/auth", Auth::handleDelete);
	factory.POST("/place", UI::PlaceHandler::handlePost);
	factory.PUT("/place/:placeId", UI::PlaceHandler::handlePut);
	factory.GET("/place/:placeId", UI::PlaceHandler::handleGet);
	factory.DELETE("/place/:placeId", UI::PlaceHandler::handleDelete);

	factory.POST("/place/:placeId/gateways/:gatewayId", UI::GatewayHandler::handlePost);
	factory.PUT("/place/:placeId/gateways/:gatewayId", UI::GatewayHandler::handlePut);
	factory.GET("/place/:placeId/gateways/:gatewayId", UI::GatewayHandler::handleGet);
	factory.DELETE("/place/:placeId/gateways/:gatewayId", UI::GatewayHandler::handleDelete);

	factory.POST("/place/:placeId/locations", UI::LocationHandler::handlePost);
	factory.GET("/place/:placeId/locations/:locationId", UI::LocationHandler::handleGet);
	factory.PUT("/place/:placeId/locations/:locationId", UI::LocationHandler::handlePut);
	factory.DELETE("/place/:placeId/locations/:locationId", UI::LocationHandler::handleDelete);
}

BEEEON_OBJECT(UIServerModule, BeeeOn::UIServerModule)

}
