#ifndef BEEEON_PLACE_DAO_H
#define BEEEON_PLACE_DAO_H

#include <Poco/Exception.h>
#include <Poco/UUIDGenerator.h>

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "model/Place.h"

namespace BeeeOn {

class PlaceDao {
public:
	virtual void create(Place &place) = 0;
	virtual bool fetch(Place &place) = 0;
	virtual bool update(Place &place) = 0;
	virtual bool remove(const Place &place) = 0;
};

class NullPlaceDao : public AbstractInjectorTarget,
		public NullDao<Place, PlaceDao> {
};

class MockPlaceDao : public AbstractInjectorTarget,
		public MockDao<Place, PlaceDao> {

protected:
	PlaceID nextID()
	{
		return PlaceID(Poco::UUIDGenerator::defaultGenerator()
				.createRandom());
	}
};

}

#endif
