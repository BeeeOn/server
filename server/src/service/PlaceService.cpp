#include <vector>
#include <Poco/Exception.h>

#include "service/PlaceService.h"

BEEEON_OBJECT(PlaceService, BeeeOn::PlaceService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

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
