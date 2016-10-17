#include <vector>
#include <Poco/Exception.h>

#include "service/PlaceService.h"
#include "dao/PlaceDao.h"
#include "dao/RoleInPlaceDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "policy/PlaceAccessPolicy.h"

BEEEON_OBJECT(PlaceService, BeeeOn::PlaceService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

PlaceService::PlaceService():
	m_placeDao(&NullPlaceDao::instance()),
	m_roleInPlaceDao(&NullRoleInPlaceDao::instance()),
	m_verifiedIdentityDao(&NullVerifiedIdentityDao::instance()),
	m_accessPolicy(&NullPlaceAccessPolicy::instance())
{
	injector<PlaceService, PlaceDao>(
		"placeDao",
		&PlaceService::setPlaceDao
	);
	injector<PlaceService, RoleInPlaceDao>(
		"roleInPlaceDao",
		&PlaceService::setRoleInPlaceDao
	);
	injector<PlaceService, VerifiedIdentityDao>(
		"verifiedIdentityDao",
		&PlaceService::setVerifiedIdentityDao
	);
	injector<PlaceService, PlaceAccessPolicy>(
		"accessPolicy",
		&PlaceService::setAccessPolicy
	);
}

void PlaceService::setPlaceDao(PlaceDao *dao)
{
	m_placeDao = dao? dao : &NullPlaceDao::instance();
}

void PlaceService::setRoleInPlaceDao(RoleInPlaceDao *dao)
{
	m_roleInPlaceDao = dao? dao : &NullRoleInPlaceDao::instance();
}

void PlaceService::setVerifiedIdentityDao(VerifiedIdentityDao *dao)
{
	m_verifiedIdentityDao = dao? dao :
		&NullVerifiedIdentityDao::instance();
}

void PlaceService::setAccessPolicy(PlaceAccessPolicy *policy)
{
	m_accessPolicy = policy? policy : &NullPlaceAccessPolicy::instance();
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
	m_accessPolicy->assureGet(input, input.target());
	return m_placeDao->fetch(input.target());
}

bool PlaceService::update(SingleWithData<Place> &input)
{
	Place &place = input.target();

	m_accessPolicy->assureUpdate(input, place);

	if (!m_placeDao->fetch(place))
		throw NotFoundException("place does not exist");

	input.data().partial(place);
	return m_placeDao->update(place);
}

bool PlaceService::remove(Relation<Place, User> &input)
{
	Place &place = input.target();

	m_accessPolicy->assureRemove(input, place);

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
