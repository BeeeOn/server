#ifndef BEEEON_LOCATION_DAO_H
#define BEEEON_LOCATION_DAO_H

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "model/Place.h"
#include "model/Location.h"

namespace BeeeOn {

class LocationDao {
public:
	virtual Location::Ptr get(LocationID id) = 0;
};

class NullLocationDao : public AbstractInjectorTarget,
	public NullDao<Location, LocationDao> {
};
class MockLocationDao : public AbstractInjectorTarget,
	public MockDao<Location, LocationDao> {
};

}

#endif
