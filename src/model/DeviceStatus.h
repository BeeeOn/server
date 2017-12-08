#pragma once

#include <Poco/Nullable.h>
#include <Poco/Timestamp.h>

namespace BeeeOn {

/**
 * @brief Device data representing its status, i.e. dynamic
 * information that might change quite often over time.
 */
class DeviceStatus {
public:
	DeviceStatus();
	~DeviceStatus();

	void setFirstSeen(const Poco::Timestamp &at);
	const Poco::Timestamp &firstSeen() const;

	void setLastSeen(const Poco::Timestamp &at);
	const Poco::Timestamp &lastSeen() const;

	void setActiveSince(const Poco::Nullable<Poco::Timestamp> &at);
	const Poco::Nullable<Poco::Timestamp> &activeSince() const;

	bool active() const
	{
		return !activeSince().isNull();
	}

private:
	Poco::Timestamp m_firstSeen;
	Poco::Timestamp m_lastSeen;
	Poco::Nullable<Poco::Timestamp> m_activeSince;
};

}
