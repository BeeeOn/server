#ifndef BEEEON_DEVICE_H
#define BEEEON_DEVICE_H

#include <Poco/SharedPtr.h>
#include <Poco/DateTime.h>
#include <Poco/Timespan.h>
#include <Poco/Nullable.h>
#include <Poco/Util/Units.h>

#include "model/Entity.h"
#include "model/Location.h"
#include "model/Gateway.h"
#include "model/DeviceID.h"
#include "model/DeviceInfo.h"

namespace BeeeOn {

/**
 * Representation of the Device application entity.
 */
class Device : public Entity<DeviceID> {
public:
	typedef Poco::SharedPtr<Device> Ptr;

	using percent = Poco::Util::Units::Values::percent;

	Device();
	Device(const ID &id);

	void setGateway(const Gateway &gateway);
	const Gateway &gateway() const;

	void setLocation(const Location &location);
	const Location &location() const;

	void setName(const std::string &name);
	const std::string &name() const;

	void setType(const Poco::SharedPtr<DeviceInfo> type);
	const Poco::SharedPtr<DeviceInfo> type() const;

	void setRefresh(const unsigned int seconds)
	{
		setRefresh(Poco::Timespan(seconds, 0));
	}

	void setRefresh(const Poco::Timespan &refresh);
	const Poco::Timespan &refresh() const;

	void setBattery(const unsigned int battery)
	{
		setBattery(percent(battery));
	}

	void setBattery(const Poco::Nullable<percent> &battery);
	const Poco::Nullable<percent> &battery() const;

	void setSignal(const unsigned int signal)
	{
		setSignal(percent(signal));
	}

	void setSignal(const Poco::Nullable<percent> &signal);
	const Poco::Nullable<percent> &signal() const;

	void setFirstSeen(const Poco::DateTime &at);
	const Poco::DateTime &firstSeen() const;

	void setLastSeen(const Poco::DateTime &at);
	const Poco::DateTime &lastSeen() const;

	/**
	 * A device is available when it has been last seen
	 * after a certain multiple of refresh time.
	 */
	bool available(const unsigned int multiple = 3,
			const Poco::DateTime &ref = Poco::DateTime()) const;

	void setActiveSince(const Poco::Nullable<Poco::DateTime> &at);
	const Poco::Nullable<Poco::DateTime> &activeSince() const;

	bool active() const
	{
		return !activeSince().isNull();
	}

private:
	Gateway m_gateway;
	Location m_location;
	std::string m_name;
	Poco::SharedPtr<DeviceInfo> m_type;
	Poco::Timespan m_refresh;
	Poco::Nullable<percent> m_battery;
	Poco::Nullable<percent> m_signal;
	Poco::DateTime m_firstSeen;
	Poco::DateTime m_lastSeen;
	Poco::Nullable<Poco::DateTime> m_activeSince;
};

typedef Device::ID DeviceID;

}

#endif
