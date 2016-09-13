#ifndef BEEEON_GATEWAY_H
#define BEEEON_GATEWAY_H

#include <Poco/SharedPtr.h>
#include "model/Place.h"
#include "model/GatewayID.h"
#include "model/Collection.h"

namespace BeeeOn {

/**
 * Representation of a Gateway application entity.
 */
class Gateway {
public:
	typedef Poco::SharedPtr<Gateway> Ptr;
	typedef BeeeOn::Collection<Gateway> Collection;
	typedef GatewayID ID;

	Gateway():
		m_altitude(0),
		m_latitude(0),
		m_longitude(0)
	{
	}

	Gateway(const ID &id):
		m_id(id),
		m_altitude(0),
		m_latitude(0),
		m_longitude(0)
	{
	}

	Gateway(const Gateway &copy):
		m_id(copy.m_id),
		m_name(copy.m_name),
		m_place(copy.m_place),
		m_altitude(copy.m_altitude),
		m_latitude(copy.m_latitude),
		m_longitude(copy.m_longitude)
	{
	}

	Gateway(const ID &id, const Gateway &copy):
		m_id(id),
		m_name(copy.m_name),
		m_place(copy.m_place),
		m_altitude(copy.m_altitude),
		m_latitude(copy.m_latitude),
		m_longitude(copy.m_longitude)
	{
	}

	void setName(const std::string &name)
	{
		m_name = name;
	}

	const std::string &name() const
	{
		return m_name;
	}

	void setPlace(const Place &place)
	{
		m_place = place;
	}

	const Place &place() const
	{
		return m_place;
	}

	const ID &id() const
	{
		return m_id;
	}

	void setAltitude(double alt)
	{
		m_altitude = alt;
	}

	double altitude() const
	{
		return m_altitude;
	}

	void setLatitude(double lat)
	{
		m_latitude = lat;
	}

	double latitude() const
	{
		return m_latitude;
	}

	void setLongitude(double lon)
	{
		m_longitude = lon;
	}

	double longitude() const
	{
		return m_longitude;
	}

	/**
	 * Frontend API only.
	 */
	template <typename Serializer>
	void toWeb(Serializer &s) const
	{
		s.push("name", m_name);
	}

	/**
	 * Frontend API only.
	 */
	template <typename Serializer>
	void fromWeb(Serializer &s)
	{
		s.get("name", m_name);
	}

private:
	ID m_id;
	std::string m_name;
	Place m_place;
	double m_altitude;
	double m_latitude;
	double m_longitude;
};

typedef Gateway::Collection GatewayCollection;
typedef Gateway::ID GatewayID;

}

#endif
