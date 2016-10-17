#include <Poco/Exception.h>

#include "service/JSONGatewayDeserializer.h"
#include "ui/GatewayHandler.h"
#include "ui/Serializing.h"
#include "model/Place.h"
#include "model/Gateway.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;
using namespace BeeeOn::UI;

GatewayHandler::GatewayHandler()
{
	injector<GatewayHandler, GatewayService>("gatewayService",
			&GatewayHandler::setGatewayService);
	injector<GatewayHandler, GatewayAccessPolicy>("accessPolicy",
			&GatewayHandler::setAccessPolicy);
}

const string GatewayHandler::handleAssign(istream &in,
		const UserID &userId,
		const string &placeId,
		const string &gatewayId)
{
	Place place(PlaceID::parse(placeId));
	Gateway gateway(GatewayID::parse(gatewayId));
	JSONGatewayDeserializer update(in);
	RelationWithData<Gateway, Place> input(gateway, update, place);
	User user(userId);
	input.setUser(user);

	m_accessPolicy->assureAssignGateway(input, place);

	if (!m_gatewayService->assignAndUpdate(input))
		return "";

	return serialize(gateway);
}

const string GatewayHandler::handleUpdate(istream &in,
		const UserID &userId,
		const string &placeId,
		const string &gatewayId)
{
	Place place(PlaceID::parse(placeId));
	Gateway gateway(GatewayID::parse(gatewayId));
	JSONGatewayDeserializer update(in);
	RelationWithData<Gateway, Place> input(gateway, update, place);
	User user(userId);
	input.setUser(user);

	m_accessPolicy->assureUpdate(input, gateway);

	if (!m_gatewayService->updateInPlace(input)) {
		throw Exception("failed to update gateway: "
				+ gateway.id().toString());
	}

	return serialize(gateway);
}

const string GatewayHandler::handleGet(
		const UserID &userId,
		const string &placeId,
		const string &gatewayId)
{
	Place place(PlaceID::parse(placeId));
	Gateway gateway(GatewayID::parse(gatewayId));
	Relation<Gateway, Place> input(gateway, place);
	User user(userId);
	input.setUser(user);

	m_accessPolicy->assureGet(input, gateway);

	if (!m_gatewayService->fetchFromPlace(input))
		return "";

	return serialize(gateway);
}

const string GatewayHandler::handleDelete(
		const UserID &userId,
		const string &placeId,
		const string &gatewayId)
{
	Place place(PlaceID::parse(placeId));
	Gateway gateway(GatewayID::parse(gatewayId));
	Relation<Gateway, Place> input(gateway, place);
	User user(userId);
	input.setUser(user);

	m_accessPolicy->assureUnassign(input, gateway);

	if (!m_gatewayService->unassign(input))
		return "";

	return serialize(gateway);
}

BEEEON_OBJECT(GatewayHandler, BeeeOn::UI::GatewayHandler)
