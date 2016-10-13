#include <Poco/SingletonHolder.h>

#include "dao/GatewayDao.h"

BEEEON_OBJECT(NullGatewayDao, BeeeOn::NullGatewayDao)
BEEEON_OBJECT(MockGatewayDao, BeeeOn::MockGatewayDao)

using namespace std;
using namespace BeeeOn;

GatewayDao &NullGatewayDao::instance()
{
	static Poco::SingletonHolder<NullGatewayDao> singleton;
	return *singleton.get();
}

MockGatewayDao::MockGatewayDao():
	m_roleInPlaceDao(&NullRoleInPlaceDao::instance())
{
	injector<MockGatewayDao, RoleInPlaceDao>("roleInPlaceDao",
			&MockGatewayDao::setRoleInPlaceDao);
}

void MockGatewayDao::fetchAccessible(
		vector<Gateway> &gateways,
		const User &user)
{
	vector<Place> places;

	m_roleInPlaceDao->fetchAccessiblePlaces(places, user);

	for (auto place : places) {
		for (auto pair : storage()) {
			const Gateway &gateway = *pair.second;

			if (!gateway.hasPlace())
				continue;

			if (gateway.place().id() != place.id())
				continue;

			gateways.push_back(gateway);
		}
	}
}
