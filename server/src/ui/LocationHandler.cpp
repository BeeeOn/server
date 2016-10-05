#include <Poco/Exception.h>

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
			const std::string &placeId)
{
	Place place(PlaceID::parse(placeId));

	Location location;
	deserialize(in, location);

	m_locationService->createIn(location, place);

	return serialize(location);
}

const string LocationHandler::handleUpdate(std::istream &in,
			const std::string &placeId,
			const std::string &locationId)
{
	Place place(PlaceID::parse(placeId));
	Location location(LocationID::parse(locationId));

	if (!m_locationService->fetchFrom(location, place))
		return "";

	deserialize(in, location);

	if (!m_locationService->update(location)) {
		throw InvalidArgumentException("failed to update location: "
				+ location.id().toString());
	}

	return serialize(location);
}

const string LocationHandler::handleGet(
		const std::string &placeId,
		const std::string &locationId)
{
	Place place(PlaceID::parse(placeId));
	Location location(LocationID::parse(locationId));

	if (!m_locationService->fetchFrom(location, place))
		return "";

	return serialize(location);
}

const string LocationHandler::handleDelete(
		const std::string &placeId,
		const std::string &locationId)
{
	Place place(PlaceID::parse(placeId));
	Location location(LocationID::parse(locationId));

	if (!m_locationService->fetchFrom(location, place))
		return "";

	if (!m_locationService->remove(location))
		return "";

	return serialize(location);
}

BEEEON_OBJECT(LocationHandler, BeeeOn::UI::LocationHandler)
