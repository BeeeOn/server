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
	User user(userId);

	m_accessPolicy->assureAssignGateway(user, place);

	if (!m_gatewayService->fetch(gateway))
		return "";

	deserialize(in, gateway);

	if (!m_gatewayService->assignAndUpdate(gateway, place))
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
	User user(userId);

	m_accessPolicy->assureUpdate(user, gateway);

	if (!m_gatewayService->updateInPlace(gateway, update, place)) {
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
	User user(userId);

	m_accessPolicy->assureGet(user, gateway);

	if (!m_gatewayService->fetchFromPlace(gateway, place))
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
	User user(userId);

	m_accessPolicy->assureUnassign(user, gateway);

	if (!m_gatewayService->unassign(gateway, place))
		return "";

	return serialize(gateway);
}

BEEEON_OBJECT(GatewayHandler, BeeeOn::UI::GatewayHandler)
