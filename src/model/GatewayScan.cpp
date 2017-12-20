#include "model/GatewayScan.h"

using namespace Poco;
using namespace BeeeOn;

GatewayScan::GatewayScan(State state):
	m_state(state),
	m_duration(2 * Timespan::MINUTES)
{
}

void GatewayScan::setState(State state)
{
	m_state = state;
}

GatewayScan::State GatewayScan::state() const
{
	return m_state;
}

GatewayScan::State GatewayScan::changeState(State state)
{
	switch (m_state) {
	case GatewayScan::SCAN_IDLE:
		m_state = state;
		break;

	case GatewayScan::SCAN_WAITING:
		if (state == GatewayScan::SCAN_IDLE)
			break;

		m_state = state;
		break;

	case GatewayScan::SCAN_PROCESSING:
		if (state == GatewayScan::SCAN_IDLE)
			break;
			
		if (state == GatewayScan::SCAN_WAITING)
			break;

		m_state = state;
		break;

	case GatewayScan::SCAN_FINISHED:
		if (state == GatewayScan::SCAN_FAILED)
			break;

		m_state = state;
		break;

	case GatewayScan::SCAN_FAILED:
		if (state == GatewayScan::SCAN_FINISHED)
			break;

		m_state = state;
		break;
	}

	return m_state;
}

void GatewayScan::setDuration(const Timespan &duration)
{
	m_duration = duration;
}

const Timespan &GatewayScan::duration() const
{
	return m_duration;
}

void GatewayScan::setStarted(const Timestamp &started)
{
	m_started = started;
}

const Timestamp &GatewayScan::started() const
{
	return m_started;
}

GatewayScan GatewayScan::createIdle()
{
	GatewayScan scan(SCAN_IDLE);
	scan.setDuration(0);
	scan.setStarted(0);

	return scan;
}
