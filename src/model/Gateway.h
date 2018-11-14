#pragma once

#include <Poco/DateTime.h>
#include <Poco/Nullable.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/IPAddress.h>

#include "l10n/TimeZone.h"
#include "model/Entity.h"
#include "model/GatewayID.h"

namespace BeeeOn {

/**
 * Representation of a Gateway application entity.
 */
class Gateway : public Entity<GatewayID> {
public:
	typedef Poco::SharedPtr<Gateway> Ptr;

	Gateway();
	Gateway(const ID &id);

	void setName(const std::string &name);
	const std::string &name() const;

	void setAltitude(const Poco::Nullable<int> alt);
	void setAltitude(int alt);
	Poco::Nullable<int> altitude() const;

	void setLatitude(double lat);
	double latitude() const;

	void setLongitude(double lon);
	double longitude() const;

	void setLastChanged(const Poco::Nullable<Poco::DateTime> &at);
	Poco::Nullable<Poco::DateTime> lastChanged() const;

	/**
	 * @brief Last activity of the gateway when connected to the
	 * gateway server. If it is not currently connected, the value
	 * should be null.
	 */
	void setLastActivity(const Poco::Nullable<Poco::DateTime> &at);
	Poco::Nullable<Poco::DateTime> lastActivity() const;

	void setVersion(const std::string &version);
	std::string version() const;

	void setIPAddress(const std::string &ipAddress);
	void setIPAddress(const Poco::Net::IPAddress &ipAddress);
	Poco::Net::IPAddress ipAddress() const;

	void setTimeZone(const TimeZone &tz);
	const TimeZone &timeZone() const;

private:
	std::string m_name;
	Poco::Nullable<int> m_altitude;
	double m_latitude;
	double m_longitude;
	Poco::Nullable<Poco::DateTime> m_lastChanged;
	Poco::Nullable<Poco::DateTime> m_lastActivity;
	std::string m_version;
	Poco::Net::IPAddress m_ipAddress;
	TimeZone m_timeZone;
};

typedef Gateway::ID GatewayID;

}
