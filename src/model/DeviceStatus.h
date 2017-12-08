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
	enum State {
		/**
		 * A new device is always initially in the inactive state.
		 * Information about such device can be cached temporarily
		 * and dropped after some time.
		 */
		STATE_INACTIVE,
		/**
		 * An active device can be deactivated. However, such operation
		 * can take time, until the appropriate gateway is contacted and
		 * successfully requested to deactivate such device. In that case,
		 * such device is waiting for deactivation.
		 */
		STATE_INACTIVE_PENDING,
		/**
		 * A device can be activated. This means that its data are collected
		 * and it is possible to control such device.
		 */
		STATE_ACTIVE,
		/**
		 * An inactive device can be activated which may take some time
		 * until the appropriate gateway is contacted and successfully
		 * requested to activate such device.
		 */
		STATE_ACTIVE_PENDING,
	};

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

	void setState(State state);
	State state() const;

private:
	Poco::Timestamp m_firstSeen;
	Poco::Timestamp m_lastSeen;
	Poco::Nullable<Poco::Timestamp> m_activeSince;
	State m_state;
};

}
