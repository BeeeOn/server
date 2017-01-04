#ifndef BEEEON_PLACE_DAO_H
#define BEEEON_PLACE_DAO_H

#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
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
public:
	/**
	 * Provide a singleton instance to avoid
	 * unnecessary allocations.
	 */
	static PlaceDao &instance();
};

}

#endif
