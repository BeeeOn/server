#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "model/Gateway.h"

namespace BeeeOn {

/**
 * @brief Representation of a gateway scan. We keep tracking of all scannings
 * of all gateways and watch their status. The state is accessible by multiple
 * users and so it is possible to start only 1 scanning on a single gateway.
 * When the scan finishes, the state can be maintained for some time holding
 * the result. The lifetime however depends on the used strategy than.
 */
class GatewayScan {
public:
	enum State {
		SCAN_IDLE,
		SCAN_WAITING,
		SCAN_PROCESSING,
		SCAN_FINISHED,
		SCAN_FAILED,
	};

	GatewayScan(State state = SCAN_WAITING);

	void setState(State state);
	State state() const;

	/**
	 * Perform a transition into the given state if possible. Otherwise
	 * stay in the current state.
	 *
	 * @return new state
	 */
	State changeState(State state);

	void setDuration(const Poco::Timespan &duration);
	const Poco::Timespan &duration() const;

	void setStarted(const Poco::Timestamp &started);
	const Poco::Timestamp &started() const;

	static GatewayScan createIdle();

private:
	State m_state;
	Poco::Timespan m_duration;
	Poco::Timestamp m_started;
};

}
