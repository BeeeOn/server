#ifndef BEEEON_PLACE_SERVICE_H
#define BEEEON_PLACE_SERVICE_H

#include "di/InjectorTarget.h"
#include "model/Place.h"
#include "dao/PlaceDao.h"

namespace BeeeOn {

class PlaceService : public AbstractInjectorTarget {
public:
	PlaceService():
		m_placeDao(&NullPlaceDao::instance())
	{
		injector<PlaceService, PlaceDao>("placeDao",
			&PlaceService::setPlaceDao);
	}

	void setPlaceDao(PlaceDao *dao)
	{
		if (dao == NULL)
			m_placeDao = &NullPlaceDao::instance();
		else
			m_placeDao = dao;
	}

	void create(Place &place)
	{
		m_placeDao->create(place);
	}

	bool fetch(Place &place)
	{
		return m_placeDao->fetch(place);
	}

	bool update(Place &place)
	{
		return m_placeDao->update(place);
	}

	bool remove(Place &place)
	{
		if (!m_placeDao->fetch(place))
			return false;

		return m_placeDao->remove(place);
	}

private:
	PlaceDao *m_placeDao;
};

}

#endif
