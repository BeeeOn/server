#include "service/LocationService.h"

BEEEON_OBJECT(LocationService, BeeeOn::LocationService)

using namespace BeeeOn;

LocationService::LocationService():
	m_dao(&NullLocationDao::instance())
{
	injector<LocationService, LocationDao>("locationDao",
			&LocationService::setLocationDao);
}

void LocationService::createIn(Location &location, const Place &place)
{
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

bool LocationService::update(Location &location)
{
	return m_dao->update(location);
}

bool LocationService::remove(const Location &location)
{
	return m_dao->remove(location);
}
