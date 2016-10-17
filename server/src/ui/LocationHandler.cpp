#include <Poco/Exception.h>

#include "server/Session.h"
#include "service/JSONLocationDeserializer.h"
#include "ui/LocationHandler.h"
#include "ui/Serializing.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;
using namespace BeeeOn::UI;

LocationHandler::LocationHandler()
{
	injector<LocationHandler, LocationService>("locationService",
			&LocationHandler::setLocationService);
}

const string LocationHandler::handleCreate(std::istream &in,
			const UserID &userId,
			const std::string &placeId)
{
	Place place(PlaceID::parse(placeId));
	User user(userId);
	JSONLocationDeserializer data(in);
	Location location;
	RelationWithData<Location, Place> input(location, data, place);
	input.setUser(user);

	m_locationService->createIn(input);

	return serialize(location);
}

const string LocationHandler::handleUpdate(std::istream &in,
			const UserID &userId,
			const std::string &placeId,
			const std::string &locationId)
{
	Place place(PlaceID::parse(placeId));
	Location location(LocationID::parse(locationId));
	JSONLocationDeserializer update(in);
	RelationWithData<Location, Place> input(location, update, place);
	User user(userId);
	input.setUser(user);

	if (!m_locationService->updateIn(input)) {
		throw InvalidArgumentException("failed to update location: "
				+ location.id().toString());
	}

	return serialize(location);
}

const string LocationHandler::handleGet(
		const UserID &userId,
		const std::string &placeId,
		const std::string &locationId)
{
	Place place(PlaceID::parse(placeId));
	Location location(LocationID::parse(locationId));
	User user(userId);
	Relation<Location, Place> input(location, place);
	input.setUser(user);

	if (!m_locationService->fetchFrom(input))
		return "";

	return serialize(location);
}

const string LocationHandler::handleDelete(
		const UserID &userId,
		const std::string &placeId,
		const std::string &locationId)
{
	Place place(PlaceID::parse(placeId));
	Location location(LocationID::parse(locationId));
	Relation<Location, Place> input(location, place);
	User user(userId);
	input.setUser(user);

	if (!m_locationService->removeFrom(input))
		return "";

	return serialize(location);
}

BEEEON_OBJECT(LocationHandler, BeeeOn::UI::LocationHandler)
