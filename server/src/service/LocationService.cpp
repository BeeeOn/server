#include <Poco/Exception.h>

#include "service/LocationService.h"

BEEEON_OBJECT(LocationService, BeeeOn::LocationService)

using namespace Poco;
using namespace BeeeOn;

LocationService::LocationService():
	m_dao(&NullLocationDao::instance())
{
	injector<LocationService, LocationDao>("locationDao",
			&LocationService::setLocationDao);
}

void LocationService::createIn(Location &location,
		const Deserializer<Location> &data,
		const Place &place)
{
	data.full(location);
	location.setPlace(place);
	m_dao->create(location);
}

bool LocationService::fetch(Location &location)
{
	return m_dao->fetch(location);
}

bool LocationService::fetchFrom(Location &location, const Place &place)
{
	return m_dao->fetchFrom(location, place);
}

bool LocationService::updateIn(Location &location,
		const Deserializer<Location> &update,
		const Place &place)
{
	if (!m_dao->fetchFrom(location, place))
		throw NotFoundException("location does not exist");

	update.partial(location);

	return m_dao->update(location);
}

bool LocationService::remove(Location &location)
{
	if (!m_dao->fetch(location))
		throw NotFoundException("location does not exist");

	return m_dao->remove(location);
}

bool LocationService::removeFrom(Location &location, const Place &place)
{
	if (!m_dao->fetchFrom(location, place))
		throw NotFoundException("location does not exist");

	return m_dao->remove(location);
}
