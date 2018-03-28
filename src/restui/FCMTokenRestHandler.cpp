#include <Poco/Exception.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "model/FCMToken.h"
#include "model/User.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "restui/FCMTokenRestHandler.h"
#include "restui/Serializing.h"
#include "util/JsonUtil.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, FCMTokenRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_PROPERTY("fcmTokenService", &FCMTokenRestHandler::setFCMTokenService)
BEEEON_OBJECT_PROPERTY("senderId", &FCMTokenRestHandler::setSenderID)
BEEEON_OBJECT_END(BeeeOn, RestUI, FCMTokenRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

FCMTokenRestHandler::FCMTokenRestHandler():
	JSONRestHandler("notifications")
{
	registerAction<FCMTokenRestHandler>("list_services", &FCMTokenRestHandler::listServices);
	registerAction<FCMTokenRestHandler>("register", &FCMTokenRestHandler::registerToken);
	registerAction<FCMTokenRestHandler>("unregister", &FCMTokenRestHandler::unregisterToken, {"token"});
}

void FCMTokenRestHandler::setFCMTokenService(FCMTokenService::Ptr service)
{
	m_service = service;
}

void FCMTokenRestHandler::listServices(RestFlow &flow)
{
	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);

	result.startArray();

	if (!senderID().empty()) {
		result.startObject();

		result.key("name");
		result.value(string("fcm"));

		result.key("id");
		result.value(senderID());

		result.endObject();
	}

	result.endArray();

	endSuccess(result);
}

FCMToken FCMTokenRestHandler::extractToken(RestFlow &flow) const
{
	User user(flow.session()->userID());
	Object::Ptr object = parseInput(flow);

	FCMToken token = FCMTokenID::parse(
		Sanitize::token(object->getValue<string>("id")));
	token.setUser(user);

	return token;
}

void FCMTokenRestHandler::registerToken(RestFlow &flow)
{
	User user(flow.session()->userID());
	FCMToken token = extractToken(flow);

	Single<FCMToken> data(token);
	data.setUser(user);

	m_service->registerToken(data);

	flow.response().setStatus(204);
}

void FCMTokenRestHandler::unregisterToken(RestFlow &flow)
{
	User user(flow.session()->userID());
	FCMToken token = FCMTokenID::parse(
		Sanitize::token(flow.param("token"))
	);
	token.setUser(user);

	Single<FCMToken> data(token);
	data.setUser(user);

	m_service->unregisterToken(data);

	flow.response().setStatus(204);
}
