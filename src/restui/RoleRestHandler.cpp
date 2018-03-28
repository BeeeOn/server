#include <Poco/Exception.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "model/Gateway.h"
#include "model/RoleInGateway.h"
#include "model/LegacyRoleInGateway.h"
#include "model/User.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "restui/JSONRoleInGatewayDeserializer.h"
#include "restui/RoleRestHandler.h"
#include "restui/Serializing.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, RoleRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_PROPERTY("roleService", &RoleRestHandler::setRoleService)
BEEEON_OBJECT_PROPERTY("identityService", &RoleRestHandler::setIdentityService)
BEEEON_OBJECT_END(BeeeOn, RestUI, RoleRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

RoleRestHandler::RoleRestHandler() :
        JSONRestHandler("roles")
{
	registerAction<RoleRestHandler>("list", &RoleRestHandler::list, {"gateway_id"});
	registerAction<RoleRestHandler>("current", &RoleRestHandler::current, {"gateway_id"});
	registerAction<RoleRestHandler>("detail", &RoleRestHandler::detail, {"gateway_id", "role_id"});
	registerAction<RoleRestHandler>("create", &RoleRestHandler::create, {"gateway_id"});
	registerAction<RoleRestHandler>("update", &RoleRestHandler::update, {"gateway_id", "role_id"});
	registerAction<RoleRestHandler>("delete", &RoleRestHandler::remove, {"gateway_id", "role_id"});
}

void RoleRestHandler::setRoleService(RoleService::Ptr service)
{
	m_roleService = service;
}

void RoleRestHandler::setIdentityService(IdentityService::Ptr service)
{
	m_identityService = service;
}

void RoleRestHandler::list(RestFlow &flow)
{
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	vector<RoleInGateway> roles;

	Relation<vector<RoleInGateway>, Gateway> input(roles, gateway);
	User user(flow.session()->userID());
	input.setUser(user);

	m_roleService->list(input);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, roles);
	endSuccess(result);
}

void RoleRestHandler::detail(RestFlow &flow)
{
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	LegacyRoleInGateway role(RoleInGatewayID::parse(flow.param("role_id")));

	Relation<LegacyRoleInGateway, Gateway> input(role, gateway);
	User user(flow.session()->userID());
	input.setUser(user);

	if (!m_roleService->fetch(input))
		throw NotFoundException("no such role " + role);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, role);
	endSuccess(result);
}

void RoleRestHandler::current(RestFlow &flow)
{
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	RoleInGateway role;
	Relation<RoleInGateway, Gateway> input(role, gateway);
	User user(flow.session()->userID());
	input.setUser(user);

	VerifiedIdentity identity(flow.session()->identityID());

	if (!m_roleService->fetch(input, identity))
		throw NotFoundException("no such role");

	flow.response().setStatus(303);
	flow.response()["Location"] = flow.linker().link("roles", "detail",
		{gateway.id().toString(), role.id().toString()});
}

void RoleRestHandler::create(RestFlow &flow)
{
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));

	const Object::Ptr object = parseInput(flow);

	Identity identity;
	identity.setEmail(Sanitize::email(object->getValue<string>("identity_email")));

	AccessLevel as(AccessLevel::parse(object->getValue<string>("access_level")));

	RoleInGateway role;

	Relation<RoleInGateway, Gateway> input(role, gateway);
	User user(flow.session()->userID());
	input.setUser(user);

	m_roleService->inviteIdentity(input, identity, as);

	flow.response().setStatus(201);
	flow.response()["Location"] = flow.linker().link("roles", "detail",
		{gateway.id().toString(), role.id().toString()});
}

void RoleRestHandler::update(RestFlow &flow)
{
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	User user(flow.session()->userID());

	RoleInGateway role(RoleInGatewayID::parse(flow.param("role_id")));

	JSONRoleInGatewayDeserializer update(parseInput(flow));

	SingleWithData<RoleInGateway> input(role, update);
	input.setUser(user);

	m_roleService->update(input);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, role);
	endSuccess(result);
}

void RoleRestHandler::remove(RestFlow &flow)
{
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	User user(flow.session()->userID());

	RoleInGateway role(RoleInGatewayID::parse(flow.param("role_id")));

	Single<RoleInGateway> input(role);
	input.setUser(user);

	m_roleService->remove(input);

	flow.response().setStatus(204);
	flow.response()["Location"] = flow.linker().link("roles", "list", {gateway.id().toString()});
}
