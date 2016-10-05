#include <Poco/Exception.h>

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
		const string &placeId, const string &gatewayId)
{
	Place place(PlaceID::parse(placeId));
	Gateway gateway(GatewayID::parse(gatewayId));

	if (!m_gatewayService->fetch(gateway))
		return "";

	deserialize(in, gateway);

	if (!m_gatewayService->assignAndUpdate(gateway, place))
		return "";

	return serialize(gateway);
}

const string GatewayHandler::handleUpdate(istream &in,
		const string &placeId,
		const string &gatewayId)
{
	Place place(PlaceID::parse(placeId));
	Gateway gateway(GatewayID::parse(gatewayId));

	if (!m_gatewayService->fetchFromPlace(gateway, place))
		return "";

	deserialize(in, gateway);

	if (!m_gatewayService->update(gateway)) {
		throw Exception("failed to update gateway: "
				+ gateway.id().toString());
	}

	return serialize(gateway);
}

const string GatewayHandler::handleGet(const string &placeId,
		const string &gatewayId)
{
	Place place(PlaceID::parse(placeId));
	Gateway gateway(GatewayID::parse(gatewayId));

	if (!m_gatewayService->fetchFromPlace(gateway, place))
		return "";

	return serialize(gateway);
}

const string GatewayHandler::handleDelete(const string &placeId,
		const string &gatewayId)
{
	Place place(PlaceID::parse(placeId));
	Gateway gateway(GatewayID::parse(gatewayId));

	if (!m_gatewayService->unassign(gateway, place))
		return "";

	return serialize(gateway);
}

BEEEON_OBJECT(GatewayHandler, BeeeOn::UI::GatewayHandler)
