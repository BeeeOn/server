#include <vector>
#include <Poco/Exception.h>

#include "service/PlaceService.h"

BEEEON_OBJECT(PlaceService, BeeeOn::PlaceService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

PlaceService::PlaceService():
	m_placeDao(&NullPlaceDao::instance()),
	m_roleInPlaceDao(&NullRoleInPlaceDao::instance())
{
	injector<PlaceService, PlaceDao>("placeDao",
		&PlaceService::setPlaceDao);
	injector<PlaceService, RoleInPlaceDao>("roleInPlaceDao",
		&PlaceService::setRoleInPlaceDao);
	injector<PlaceService, VerifiedIdentityDao>(
		"verifiedIdentityDao",
		&PlaceService::setVerifiedIdentityDao
	);
}

void PlaceService::setPlaceDao(PlaceDao *dao)
{
	if (dao == NULL)
		m_placeDao = &NullPlaceDao::instance();
	else
		m_placeDao = dao;
}

void PlaceService::setRoleInPlaceDao(RoleInPlaceDao *dao)
{
	if (dao == NULL)
		m_roleInPlaceDao = &NullRoleInPlaceDao::instance();
	else
		m_roleInPlaceDao = dao;
}

void PlaceService::setVerifiedIdentityDao(VerifiedIdentityDao *dao)
{
	if (dao == NULL)
		m_verifiedIdentityDao = &NullVerifiedIdentityDao::instance();
	else
		m_verifiedIdentityDao = dao;
}

void PlaceService::create(SingleWithData<Place> &input,
		const Identity &identity)
{
	Place &place = input.target();
	input.data().full(place);
	m_placeDao->create(place);

	RoleInPlace role;
	role.setPlace(place);
	role.setIdentity(identity);
	role.setLevel(AccessLevel::admin());

	m_roleInPlaceDao->create(role);
}

void PlaceService::create(SingleWithData<Place> &input,
		VerifiedIdentity &verifiedIdentity)
{
	if (!m_verifiedIdentityDao->fetch(verifiedIdentity))
		throw InvalidAccessException("no such identity");

	create(input, verifiedIdentity.identity());
}

void PlaceService::fetchAccessible(Relation<std::vector<Place>, User> &input)
{
	m_roleInPlaceDao->fetchAccessiblePlaces(input.target(), input.base());
}

bool PlaceService::fetch(Single<Place> &input)
{
	return m_placeDao->fetch(input.target());
}

bool PlaceService::update(SingleWithData<Place> &input)
{
	Place &place = input.target();

	if (!m_placeDao->fetch(place))
		throw NotFoundException("place does not exist");

	input.data().partial(place);
	return m_placeDao->update(place);
}

bool PlaceService::remove(Relation<Place, User> &input)
{
	Place &place = input.target();

	vector<RoleInPlace> roles;
	m_roleInPlaceDao->fetchBy(roles, place);

	for (auto role : roles) {
		if (role.identity().user().id() != input.base().id()) {
			throw IllegalStateException(
				"cannot delete place "
				+ place.id().toString()
				+ " because it contains users other then "
				+ input.base().id().toString());
		}

		if (!m_roleInPlaceDao->remove(role)) {
			throw IllegalStateException(
				"failed to drop role "
				+ role.id().toString()
				+ "from place "
				+ place.id().toString());
		}
	}

	if (!m_placeDao->fetch(place))
		return false;

	return m_placeDao->remove(place);
}
