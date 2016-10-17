#include <vector>
#include <Poco/Exception.h>

#include "service/PlaceService.h"

BEEEON_OBJECT(PlaceService, BeeeOn::PlaceService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

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

void PlaceService::fetchAccessible(std::vector<Place> &places,
		const User &user)
{
	m_roleInPlaceDao->fetchAccessiblePlaces(places, user);
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

bool PlaceService::remove(Place &place, const User &owner)
{
	vector<RoleInPlace> roles;
	m_roleInPlaceDao->fetchBy(roles, place);

	for (auto role : roles) {
		if (role.identity().user().id() != owner.id()) {
			throw IllegalStateException(
				"cannot delete place "
				+ place.id().toString()
				+ " because it contains users other then "
				+ owner.id().toString());
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
