#include <Poco/Exception.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "model/User.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "restui/Serializing.h"
#include "restui/JSONLocationDeserializer.h"
#include "restui/LocationRestHandler.h"
#include "service/Relation.h"
#include "service/Single.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, LocationRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_PROPERTY("locationService", &LocationRestHandler::setLocationService)
BEEEON_OBJECT_END(BeeeOn, RestUI, LocationRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

LocationRestHandler::LocationRestHandler():
        JSONRestHandler("locations")
{
	registerAction<LocationRestHandler>("list",
			&LocationRestHandler::list, {"gateway_id"});
	registerAction<LocationRestHandler>("detail",
			&LocationRestHandler::detail, {"gateway_id", "location_id"});
	registerAction<LocationRestHandler>("create",
			&LocationRestHandler::create, {"gateway_id"});
	registerAction<LocationRestHandler>("update",
			&LocationRestHandler::update, {"gateway_id", "location_id"});
	registerAction<LocationRestHandler>("delete",
			&LocationRestHandler::remove, {"gateway_id", "location_id"});
}

void LocationRestHandler::setLocationService(LocationService::Ptr service)
{
	m_locationService = service;
}

void LocationRestHandler::list(RestFlow &flow)
{
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	vector<Location> locations;
	Relation<vector<Location>, Gateway> input(locations, gateway);
	User user(flow.session()->userID());
	input.setUser(user);

	m_locationService->fetchBy(input);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, locations);
	endSuccess(result);
}

void LocationRestHandler::detail(RestFlow &flow)
{
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Location location(LocationID::parse(flow.param("location_id")));

	Relation<Location, Gateway> input(location, gateway);
	User user(flow.session()->userID());
	input.setUser(user);

	if (!m_locationService->fetchFrom(input))
		throw NotFoundException("no such location " + location);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, location);
	endSuccess(result);
}

void LocationRestHandler::create(RestFlow &flow)
{
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));

	Location location;
	Object::Ptr object = parseInput(flow);
	JSONLocationDeserializer create(object);

	RelationWithData<Location, Gateway> input(location, create, gateway);

	User user(flow.session()->userID());
	input.setUser(user);

	m_locationService->createIn(input);

	flow.response().setStatus(201);
	flow.response()["Location"] = flow.linker().link("locations", "detail",
	{gateway.id().toString(), location.id().toString()});
}


void LocationRestHandler::update(RestFlow &flow)
{
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Location location(LocationID::parse(flow.param("location_id")));
	Object::Ptr object = parseInput(flow);
	JSONLocationDeserializer create(object);

	RelationWithData<Location, Gateway> input(location, create, gateway);
	User user(flow.session()->userID());
	input.setUser(user);

	if (!m_locationService->updateIn(input))
		throw NotFoundException("no such location " + location);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, location);
	endSuccess(result);
}

void LocationRestHandler::remove(RestFlow &flow)
{
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Location location(LocationID::parse(flow.param("location_id")));

	Relation<Location, Gateway> input(location, gateway);
	User user(flow.session()->userID());
	input.setUser(user);

	if (!m_locationService->removeFrom(input))
		throw NotFoundException("no such location " + location);

	flow.response().setStatus(204);
	flow.response()["Location"] = flow.linker().link("locations", "list", {gateway.id().toString()});
}
