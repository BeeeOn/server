#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "model/Gateway.h"
#include "model/GatewayMessage.h"
#include "model/User.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "restui/MessageRestHandler.h"
#include "restui/Serializing.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, MessageRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_PROPERTY("gatewayMessageService", &MessageRestHandler::setGatewayMessageService)
BEEEON_OBJECT_END(BeeeOn, RestUI, MessageRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

MessageRestHandler::MessageRestHandler():
	JSONRestHandler("gateway_messages")
{
	registerAction<MessageRestHandler>(
		"list", &MessageRestHandler::list, {"gateway_id"});
	registerAction<MessageRestHandler>(
		"detail", &MessageRestHandler::detail, {"gateway_id", "id"});
	registerAction<MessageRestHandler>(
		"remove", &MessageRestHandler::remove, {"gateway_id", "id"});
}

void MessageRestHandler::setGatewayMessageService(GatewayMessageService::Ptr service)
{
	m_service = service;
}

void MessageRestHandler::list(RestFlow &flow)
{
	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	vector<GatewayMessage> messages;

	Relation<vector<GatewayMessage>, Gateway> data(messages, gateway);
	data.setUser(user);

	m_service->fetchMany(data);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, *flow.translator(), messages);
	endSuccess(result);
}

void MessageRestHandler::detail(RestFlow &flow)
{
	User user(flow.session()->userID());
	GatewayMessage message(GatewayMessageID::parse(flow.param("id")));
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));

	Relation<GatewayMessage, Gateway> data(message, gateway);
	data.setUser(user);

	if (!m_service->fetch(data))
		throw NotFoundException("no such message " + message);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, *flow.translator(), message);
	endSuccess(result);
}

void MessageRestHandler::remove(RestFlow &flow)
{
	User user(flow.session()->userID());
	GatewayMessage message(GatewayMessageID::parse(flow.param("id")));
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));

	Relation<GatewayMessage, Gateway> data(message, gateway);
	data.setUser(user);

	if (!m_service->remove(data))
		throw NotFoundException("no such message: " + message);

	flow.response().setStatus(204);
	flow.response()["Location"] = flow.linker()
		.link("gateway_messages", "list", {gateway.id().toString()});
}
