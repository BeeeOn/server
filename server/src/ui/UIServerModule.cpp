#include <map>
#include <Poco/StreamCopier.h>
#include <Poco/Net/NetException.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include "service/AuthService.h"
#include "server/Route.h"
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
	const string &userData = user->toString();

	context.response().sendBuffer(userData.c_str(), userData.size());
}

static void handleCreateUser(UIRouteContext &context)
{
	std::string userData;
	StreamCopier::copyToString(context.request().stream(), userData);
	User user(userData);

	User::Ptr created = context.userData().userService().create(user);
	const string &result = created->toString();
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
}

BEEEON_OBJECT(UIServerModule, BeeeOn::UIServerModule)

}
