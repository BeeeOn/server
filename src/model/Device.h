#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Util/Units.h>

#include "model/DeviceStatus.h"
#include "model/Entity.h"
#include "model/Location.h"
#include "model/Gateway.h"
#include "model/DeviceID.h"
#include "model/DeviceInfo.h"
#include "model/RefreshTime.h"

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

	void setRefresh(const int seconds)
	{
		setRefresh(RefreshTime::fromSeconds(seconds));
	}

	void setRefresh(const RefreshTime &refresh);
	const RefreshTime &refresh() const;
	bool hasRefresh() const;

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

	void setStatus(const DeviceStatus &status);
	const DeviceStatus &status() const;
	DeviceStatus &status();

	/**
	 * A device is available when it has been last seen
	 * after a certain multiple of refresh time.
	 */
	bool available(const unsigned int multiple = 3,
			const Poco::Timestamp &ref = Poco::Timestamp()) const;

private:
	Gateway m_gateway;
	Location m_location;
	std::string m_name;
	Poco::SharedPtr<DeviceInfo> m_type;
	RefreshTime m_refresh;
	Poco::Nullable<percent> m_battery;
	Poco::Nullable<percent> m_signal;
	DeviceStatus m_status;
};

typedef Device::ID DeviceID;

}
