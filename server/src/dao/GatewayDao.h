#ifndef BEEEON_GATEWAY_DAO_H
#define BEEEON_GATEWAY_DAO_H

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "model/Place.h"
#include "model/Gateway.h"

namespace BeeeOn {

class GatewayDao {
public:
	virtual bool insert(Gateway &gateway) = 0;
	virtual bool fetch(Gateway &gateway) = 0;
	virtual bool update(Gateway &gateway) = 0;
	virtual bool assignAndUpdate(Gateway &gateway, const Place &place) = 0;
	virtual bool unassign(Gateway &gateway) = 0;
	virtual bool fetchFromPlace(Gateway &gateway, Place &place) = 0;
};

class NullGatewayDao : public AbstractInjectorTarget,
	public NullDao<Gateway, GatewayDao> {
public:
	/**
	 * Provide a singleton instance to avoid
	 * unnecessary allocations.
	 */
	static GatewayDao &instance();

	bool assignAndUpdate(Gateway &gateway, const Place &place)
	{
		return update(gateway);
	}

	bool unassign(Gateway &gateway)
	{
		return update(gateway);
	}

	bool fetchFromPlace(Gateway &gateway, Place &place)
	{
		return fetchFromPlace(gateway, place);
	}

};


class MockGatewayDao : public AbstractInjectorTarget,
	public MockDao<Gateway, GatewayDao> {
protected:
	GatewayID nextID()
	{
		return GatewayID::random();
	}

	bool assignAndUpdate(Gateway &gateway, const Place &place)
	{
		gateway.setPlace(place);
		return update(gateway);
	}

	/**
	 * Create an empty Place and set it as new place for given gateway.
	 * This generally means unassigning a gateway from place.
	 */
	bool unassign(Gateway &gateway)
	{
		Place place;
		gateway.setPlace(place);

		return update(gateway);
	}

	/**
	 * Fetch a gateway from DB and compare the given placeID with
	 * the fetched one.
	 */
	bool fetchFromPlace(Gateway &gateway, Place &place)
	{
		Gateway fetchedGateway(gateway);

		bool result = fetch(fetchedGateway);

		if (result && fetchedGateway.place().id() == place.id()) {
			gateway = fetchedGateway;
			return true;
		}

		return false;
	}

private:
	GatewayID m_id;
};

}

#endif
