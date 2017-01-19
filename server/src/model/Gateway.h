#ifndef BEEEON_GATEWAY_H
#define BEEEON_GATEWAY_H

#include <Poco/SharedPtr.h>
#include <Poco/DateTime.h>
#include <Poco/Nullable.h>
#include <Poco/Net/IPAddress.h>

#include "model/Place.h"
#include "model/GatewayID.h"

namespace BeeeOn {

/**
 * Representation of a Gateway application entity.
 */
class Gateway {
public:
	typedef Poco::SharedPtr<Gateway> Ptr;
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

	bool hasPlace() const
	{
		return !m_place.id().isNull();
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

	void setLastChanged(const Poco::Nullable<Poco::DateTime> &at)
	{
		m_lastChanged = at;
	}

	Poco::Nullable<Poco::DateTime> lastChanged() const
	{
		return m_lastChanged;
	}

	void setVersion(const std::string &version)
	{
		m_version = version;
	}

	std::string version() const
	{
		return m_version;
	}

	void setIPAddress(const std::string &ipAddress)
	{
		setIPAddress(Poco::Net::IPAddress(ipAddress));
	}

	void setIPAddress(const Poco::Net::IPAddress &ipAddress)
	{
		m_ipAddress = ipAddress;
	}

	Poco::Net::IPAddress ipAddress() const
	{
		return m_ipAddress;
	}

private:
	ID m_id;
	std::string m_name;
	Place m_place;
	double m_altitude;
	double m_latitude;
	double m_longitude;
	Poco::Nullable<Poco::DateTime> m_lastChanged;
	std::string m_version;
	Poco::Net::IPAddress m_ipAddress;
};

typedef Gateway::ID GatewayID;

}

#endif
