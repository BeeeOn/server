#include <Poco/Exception.h>

#include "ui/PlaceHandler.h"
#include "ui/Serializing.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;
using namespace BeeeOn::UI;

PlaceHandler::PlaceHandler()
{
	injector<PlaceHandler, PlaceService>("placeService",
			&PlaceHandler::setPlaceService);
	injector<PlaceHandler, IdentityService>("identityService",
			&PlaceHandler::setIdentityService);
}

const string PlaceHandler::handleCreate(istream &in,
		const VerifiedIdentityID &identityID)
{
	VerifiedIdentity identity(identityID);
	if (!m_identityService->fetch(identity))
		throw InvalidAccessException("no such identity");

	Place place;
	deserialize(in, place);

	m_placeService->create(place, identity.identity());
	return serialize(place);
}

const string PlaceHandler::handleUpdate(istream &in,
		const string &placeId)
{
	Place place(PlaceID::parse(placeId));

	if (!m_placeService->fetch(place))
		return "";

	deserialize(in, place);

	if (!m_placeService->update(place)) {
		throw Exception("failed to update place: "
				+ place.id().toString());
	}

	return serialize(place);
}

const string PlaceHandler::handleGet(const string &placeId)
{
	Place place(PlaceID::parse(placeId));

	if (!m_placeService->fetch(place)) {
		return "";
	}

	return serialize(place);
}

const string PlaceHandler::handleDelete(const string &placeId)
{
	Place place(PlaceID::parse(placeId));

	if (!m_placeService->fetch(place))
		return "";

	if (!m_placeService->remove(place))
		return "";

	return serialize(place);
}

BEEEON_OBJECT(PlaceHandler, BeeeOn::UI::PlaceHandler)
