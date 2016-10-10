#include <Poco/Exception.h>

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
	injector<LocationHandler, LocationAccessPolicy>("accessPolicy",
			&LocationHandler::setAccessPolicy);
}

const string LocationHandler::handleCreate(std::istream &in,
			const UserID &userId,
			const std::string &placeId)
{
	Place place(PlaceID::parse(placeId));
	User user(userId);
	JSONLocationDeserializer data(in);
	Location location;

	m_accessPolicy->assureCreateLocation(user, place);

	m_locationService->createIn(location, data, place);

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
	User user(userId);

	m_accessPolicy->assureUpdate(user, location);

	if (!m_locationService->updateIn(location, update, place)) {
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

	m_accessPolicy->assureGet(user, location);

	if (!m_locationService->fetchFrom(location, place))
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
	User user(userId);

	m_accessPolicy->assureRemove(user, location);

	if (!m_locationService->removeFrom(location, place))
		return "";

	return serialize(location);
}

BEEEON_OBJECT(LocationHandler, BeeeOn::UI::LocationHandler)
