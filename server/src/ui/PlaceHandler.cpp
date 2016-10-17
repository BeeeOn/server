#include <Poco/Exception.h>

#include "service/JSONPlaceDeserializer.h"
#include "service/Single.h"
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
	injector<PlaceHandler, PlaceAccessPolicy>("accessPolicy",
			&PlaceHandler::setAccessPolicy);
}

const string PlaceHandler::handleCreate(istream &in,
		const VerifiedIdentityID &identityID)
{
	VerifiedIdentity identity(identityID);
	JSONPlaceDeserializer data(in);
	Place place;
	SingleWithData<Place> input(place, data);

	m_placeService->create(input, identity);
	return serialize(place);
}

const string PlaceHandler::handleUpdate(istream &in,
		const UserID &userId,
		const string &placeId)
{
	Place place(PlaceID::parse(placeId));
	JSONPlaceDeserializer update(in);
	User user(userId);
	SingleWithData<Place> input(place, update);

	m_accessPolicy->assureUpdate(user, place);

	if (!m_placeService->update(input)) {
		throw Exception("failed to update place: "
				+ place.id().toString());
	}

	return serialize(place);
}

const string PlaceHandler::handleGet(const UserID &userId,
		const string &placeId)
{
	Place place(PlaceID::parse(placeId));
	Single<Place> input(place);
	User user(userId);

	m_accessPolicy->assureGet(user, place);

	if (!m_placeService->fetch(input)) {
		return "";
	}

	return serialize(place);
}

const string PlaceHandler::handleDelete(const UserID &userId,
		const string &placeId)
{
	Place place(PlaceID::parse(placeId));
	User user(userId);

	m_accessPolicy->assureRemove(user, place);

	if (!m_placeService->remove(place, user))
		return "";

	return serialize(place);
}

const string PlaceHandler::handleGetAll(const UserID &userId)
{
	User user(userId);
	std::vector<Place> places;

	m_placeService->fetchAccessible(places, user);
	return serialize(places);
}

BEEEON_OBJECT(PlaceHandler, BeeeOn::UI::PlaceHandler)
