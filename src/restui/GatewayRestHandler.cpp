#include <Poco/Exception.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "model/User.h"
#include "model/Gateway.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "restui/GatewayRestHandler.h"
#include "restui/JSONGatewayDeserializer.h"
#include "restui/Serializing.h"
#include "service/GatewayService.h"
#include "util/JsonUtil.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, GatewayRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_REF("gatewayService", &GatewayRestHandler::setGatewayService)
BEEEON_OBJECT_REF("timeZoneProvider", &GatewayRestHandler::setTimeZoneProvider)
BEEEON_OBJECT_END(BeeeOn, RestUI, GatewayRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

GatewayRestHandler::GatewayRestHandler():
	JSONRestHandler("gateways")
{
	registerAction<GatewayRestHandler>("list", &GatewayRestHandler::list);
	registerAction<GatewayRestHandler>("detail", &GatewayRestHandler::detail, {"gatewayId"});
	registerAction<GatewayRestHandler>("update", &GatewayRestHandler::update, {"gatewayId"});
	registerAction<GatewayRestHandler>("assign", &GatewayRestHandler::assign);
	registerAction<GatewayRestHandler>("unassign", &GatewayRestHandler::unassign, {"gatewayId"});
}

void GatewayRestHandler::setGatewayService(GatewayService *service)
{
	m_service = service;
}

void GatewayRestHandler::setTimeZoneProvider(TimeZoneProvider::Ptr provider)
{
	m_timeZoneProvider = provider;
}

void GatewayRestHandler::list(RestFlow &flow)
{
	vector<Gateway> gateways;
	User user(flow.session()->userID());
	Relation<vector<Gateway>, User> data(gateways, user);
	data.setUser(user);

	m_service->fetchAccessible(data);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, gateways, flow.locale());
	endSuccess(result);
}

void GatewayRestHandler::detail(RestFlow &flow)
{
	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gatewayId")));
	Single<Gateway> data(gateway);
	data.setUser(user);

	if (!m_service->fetch(data))
		throw NotFoundException("no such gateway " + gateway);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, gateway, flow.locale());
	endSuccess(result);
}

void GatewayRestHandler::update(RestFlow &flow)
{
	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gatewayId")));

	JSONGatewayDeserializer update(parseInput(flow));
	update.setTimeZoneProvider(m_timeZoneProvider);

	SingleWithData<Gateway> data(gateway, update);
	data.setUser(user);

	if (!m_service->update(data))
		throw IllegalStateException("failed to update gateway " + gateway);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, gateway, flow.locale());
	endSuccess(result);
}

void GatewayRestHandler::assign(RestFlow &flow)
{
	User user(flow.session()->userID());
	VerifiedIdentity identity(flow.session()->identityID());
	Object::Ptr object = parseInput(flow);
	Gateway gateway(GatewayID::parse(object->getValue<string>("id")));

	JSONGatewayDeserializer create(object);
	create.setTimeZoneProvider(m_timeZoneProvider);

	SingleWithData<Gateway> data(gateway, create);
	data.setUser(user);

	if (!m_service->registerGateway(data, identity))
		throw NotFoundException("no such gateway " + gateway);

	flow.response().setStatus(201);
	flow.response()["Location"] =
		flow.linker().link("gateways", "detail", {gateway.id().toString()});
}

void GatewayRestHandler::unassign(RestFlow &flow)
{
	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gatewayId")));
	Single<Gateway> data(gateway);
	data.setUser(user);

	if (!m_service->unregister(data))
		throw NotFoundException("no such gateway " + gateway);

	flow.response().setStatus(204);
	flow.response()["Location"] = flow.linker().link("gateways", "list");
}
