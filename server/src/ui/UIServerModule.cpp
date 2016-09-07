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

static void handleGetUser(UIRouteContext &context)
{
	UIRoute::Params::const_iterator it;

	it = context.params().find("userId");
	if (it == context.params().end()) {
		context.response().setStatus(UIResponse::HTTP_NOT_FOUND);
		context.response().setReason("Not found");
		return;
	}

	User user(UserID::parse(it->second));
	if (!context.userData().userService().fetch(user)) {
		context.response().setStatus(UIResponse::HTTP_NOT_FOUND);
		context.response().setReason("Not found");
		return;
	}

	JSONObjectSerializer serializer;
	user.toWeb(serializer);
	const string &userData = serializer.toString();

	context.response().sendBuffer(userData.c_str(), userData.size());
}

static void handleCreateUser(UIRouteContext &context)
{
	std::string userData;
	StreamCopier::copyToString(context.request().stream(), userData);
	JSONObjectSerializer deserializer;
	User user;
	user.fromWeb(deserializer);

	context.userData().userService().create(user);
	JSONObjectSerializer serializer;
	user.toWeb(serializer);
	const string &result = serializer.toString();

	context.response().sendBuffer(result.c_str(), result.size());
}

static void handleGetDevice(UIRouteContext &context)
{
	UIRoute::Params::const_iterator it;

	it = context.params().find("placeId");
	if (it == context.params().end()) {
		context.response().setStatus(UIResponse::HTTP_NOT_FOUND);
		context.response().setReason("Not found");
		return;
	}

	Place place(PlaceID::parse(it->second));

	it = context.params().find("deviceId");
	if (it == context.params().end()) {
		context.response().setStatus(UIResponse::HTTP_NOT_FOUND);
		context.response().setReason("Not found");
		return;
	}

	Device device(DeviceID::parse(it->second));
	if (!context.userData().deviceService().fetch(device)) {
		context.response().setStatus(UIResponse::HTTP_NOT_FOUND);
		context.response().setReason("Not found");
		return;
	}

	JSONObjectSerializer serializer;
	device.toWeb(serializer);
	const string &result = serializer.toString();

	context.response().sendBuffer(result.c_str(), result.size());
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

	factory.POST("/users", handleCreateUser);
	factory.GET("/users/:userId", handleGetUser);
	factory.GET("/:placeId/devices/:deviceId", handleGetDevice);

	factory.POST("/place/:placeId/locations", UI::LocationHandler::handlePost);
	factory.GET("/place/:placeId/locations/:locationId", UI::LocationHandler::handleGet);
	factory.PUT("/place/:placeId/locations/:locationId", UI::LocationHandler::handlePut);
	factory.DELETE("/place/:placeId/locations/:locationId", UI::LocationHandler::handleDelete);
}

BEEEON_OBJECT(UIServerModule, BeeeOn::UIServerModule)

}
