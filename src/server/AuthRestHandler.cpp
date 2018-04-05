#include <Poco/Exception.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/Net/NetException.h>
#include <Poco/JSON/PrintHandler.h>
#include <Poco/JSON/Object.h>

#include "di/Injectable.h"
#include "provider/AuthProvider.h"
#include "provider/OAuth2AuthProvider.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "server/AuthRestHandler.h"
#include "service/AuthService.h"
#include "util/JsonUtil.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, AuthRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_PROPERTY("authService", &AuthRestHandler::setAuthService)
BEEEON_OBJECT_END(BeeeOn, AuthRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace BeeeOn;

AuthRestHandler::AuthRestHandler():
	JSONRestHandler("auth")
{
	registerAction("list", &AuthRestHandler::list);
	registerAction("login", &AuthRestHandler::login);
	registerAction("logout", &AuthRestHandler::logout);
}

void AuthRestHandler::setAuthService(AuthService *service)
{
	m_authService = service;
}

void AuthRestHandler::list(RestFlow &flow)
{
	vector<AuthProvider *> providers;
	m_authService->list(providers);

	PrintHandler result(flow.response().stream());
	beginSuccess(result);
	result.startArray();

	for (auto provider : providers) {
		result.startObject();

		result.key("id");
		result.value(provider->id());

		result.key("name");
		result.value(provider->name()); // TODO

		result.key("display_name");
		result.value(flow.translator()->format(
			"auth.providers." + provider->name(),
			provider->name()
		));

		OAuth2AuthProvider *oauth2 = dynamic_cast<OAuth2AuthProvider *>(provider);
		if (oauth2 != NULL) {
			result.key("oauth2");
			result.startObject();
			result.key("client_id");
			result.value(oauth2->clientId());

			if (!oauth2->redirectURI().empty()) {
				result.key("redirect_uri");
				result.value(oauth2->redirectURI());
			}

			result.endObject();
		}

		result.endObject();
	}

	result.endArray();
	endSuccess(result);
}

void AuthRestHandler::login(RestFlow &flow)
{
	Object::Ptr object;
	
	try {
		object = parseInput(flow);
	}
	catch (const JSONException &e) {
		throw NotAuthenticatedException("invalid login credentials", e);
	}

	if (!object->has("provider"))
		throw NotAuthenticatedException("malformed input, missing provider");

	const string &provider = Sanitize::strict(
			object->getValue<string>("provider"));
	Session::Ptr session;

	if (object->has("code")) {
		AuthCodeCredentials credentials(
			provider,
			object->getValue<string>("code")
		);

		session = doLogin(credentials);
	}
	else if (object->has("key")) {
		ApiKeyCredentials credentials(
			provider,
			Sanitize::token(object->getValue<string>("key"))
		);

		session = doLogin(credentials);
	}
	else {
		throw NotAuthenticatedException("malformed input, missing credentials");
	}

	if (session.isNull())
		throw InvalidAccessException("login failed");

	PrintHandler result(flow.response().stream());

	beginSuccess(result);
	result.startObject();
	result.key("id");
	result.value(session->sessionID());
	result.endObject();
	endSuccess(result);
}

Session::Ptr AuthRestHandler::doLogin(const Credentials &credentials)
{
	Session::Ptr session;

	try {
		session	= m_authService->login(credentials);
	}
	catch (const Exception &e) {
		throw NotAuthenticatedException("login failed", e);
	}

	return session;
}

void AuthRestHandler::logout(RestFlow &flow)
{
	m_authService->logout(flow.session()->sessionID());

	flow.response().setStatus(204);
	flow.response()["Location"] = flow.linker().link("auth", "login");
}
