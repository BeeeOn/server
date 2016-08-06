#include <map>
#include <Poco/StreamCopier.h>
#include <Poco/Net/NetException.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include "service/AuthService.h"
#include "server/Route.h"
#include "model/JSONSerializer.h"
#include "UIServerModule.h"
#include "Debug.h"

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

	throw Poco::Net::NotAuthenticatedException("not implemented");
}

void handleAuth(UIRouteContext &context)
{
	TRACE_FUNC();

	string scheme;
	string authInfo;
	context.request().getCredentials(scheme, authInfo);

	string jsonData;
	StreamCopier::copyToString(context.request().stream(), jsonData);

	Parser parser;
	parser.parse(jsonData);
	const Var result = parser.result();
	const Object::Ptr data = result.extract<Object::Ptr>();
	const Var provider = data->get("provider");
	const Var token = data->get("token");

	map<string, string> info;
	info.insert(make_pair("provider", provider.toString()));
	info.insert(make_pair("token", token.toString()));

	Credentials cred(scheme, authInfo, info);
	const string &sessionId = context.userData()
					.authService().login(cred);

	context.response().send() << sessionId;
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

	UserID id = stoi(it->second);
	const User::Ptr user = context.userData().userService().get(id);

	JSONObjectSerializer serializer;
	user->toWeb(serializer);
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

	User::Ptr created = context.userData().userService().create(user);
	JSONObjectSerializer serializer;
	created->toWeb(serializer);
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

	PlaceID placeId = stoi(it->second);

	it = context.params().find("deviceId");
	if (it == context.params().end()) {
		context.response().setStatus(UIResponse::HTTP_NOT_FOUND);
		context.response().setReason("Not found");
		return;
	}

	DeviceID id = stoi(it->second);

	const Device::Ptr device = context
		.userData().deviceService().get(id, placeId);
	JSONObjectSerializer serializer;
	device->toWeb(serializer);
	const string &result = serializer.toString();

	context.response().sendBuffer(result.c_str(), result.size());
}

namespace BeeeOn {

void factorySetup(UIServerRequestHandlerFactory &factory)
{
	factory.noRoute(handleNoRoute);
	factory.noOperation(handleNoOperation);
	factory.sessionVerifier(verifyAuthorized);
	factory.POST("/auth", handleAuth, false);
	factory.DELETE("/auth", handleAuth);
	factory.POST("/users", handleCreateUser);
	factory.GET("/users/:userId", handleGetUser);
	factory.GET("/:placeId/devices/:deviceId", handleGetDevice);
}

BEEEON_OBJECT(UIServerModule, BeeeOn::UIServerModule)

}
