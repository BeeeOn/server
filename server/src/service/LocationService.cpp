#include <Poco/Exception.h>

#include "service/LocationService.h"
#include "dao/GatewayDao.h"

BEEEON_OBJECT(LocationService, BeeeOn::LocationService)

using namespace Poco;
using namespace BeeeOn;

LocationService::LocationService():
	m_dao(&NullLocationDao::instance()),
	m_gatewayDao(&NullGatewayDao::instance())
{
	injector<LocationService, LocationDao>("locationDao",
			&LocationService::setLocationDao);
	injector<LocationService, GatewayDao>("gatewayDao",
			&LocationService::setGatewayDao);
}

void LocationService::setGatewayDao(GatewayDao *dao)
{
	m_gatewayDao = dao? dao : &NullGatewayDao::instance();
}

void LocationService::createIn(Location &location,
		const Deserializer<Location> &data,
		const Place &place)
{
	data.full(location);
	location.setPlace(place);
	m_dao->create(location);
}

void LocationService::createIn(Location &location,
		const Deserializer<Location> &data,
		const Gateway &gateway)
{
	Gateway tmp(gateway);

	if (!m_gatewayDao->fetch(tmp))
		throw NotFoundException("gateway does not exist");

	if (!tmp.hasPlace()) // do not leak it exists
		throw NotFoundException("gateway is not assigned");

	data.full(location);
	location.setPlace(tmp.place());
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

bool LocationService::updateIn(Location &location,
		const Deserializer<Location> &update,
		const Gateway &gateway)
{
	if (!m_dao->fetchFrom(location, gateway))
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
