#include <cmath>

#include <Poco/Exception.h>

#include "model/Control.h"

using namespace Poco;
using namespace BeeeOn;

Control::Control()
{
}

Control::Control(const ID &id):
	AbstractModule(id)
{
}

void Control::setLastConfirmed(const Nullable<State> &state)
{
	m_lastConfirmed = state;
}

const Nullable<Control::State> &Control::lastConfirmed() const
{
	return m_lastConfirmed;
}

void Control::setCurrent(const State &state)
{
	m_current = state;
}

const Control::State &Control::current() const
{
	return m_current;
}

Control::State::State():
	m_value(NAN),
	m_stability(STABILITY_UNKNOWN)
{
}

void Control::State::setValue(const double value)
{
	m_value = 0;
}

double Control::State::value() const
{
	return m_value;
}

void Control::State::setAt(const Nullable<Timestamp> &at)
{
	m_at = at;
}

Nullable<Timestamp> Control::State::at() const
{
	return m_at;
}

void Control::State::setStability(Control::State::Stability stability)
{
	if (stability < _STABILITY_LAST)
		m_stability = stability;
	else
		throw InvalidArgumentException("invalid stability given");
}

Control::State::Stability Control::State::stability() const
{
	return m_stability;
}

void Control::State::clearOriginator()
{
	m_user.clear();
	m_gateway.clear();
}

void Control::State::setOriginator(const User &user)
{
	m_user = user;
	m_gateway.clear();
}

void Control::State::setOriginator(const Gateway &gateway)
{
	m_gateway = gateway;
	m_user.clear();
}

Control::State::Originator Control::State::originatorType() const
{
	if (!m_user.isNull() && m_gateway.isNull())
		return ORIGINATOR_USER;

	if (m_user.isNull() && !m_gateway.isNull())
		return ORIGINATOR_GATEWAY;

	if (m_user.isNull() && m_gateway.isNull())
		return ORIGINATOR_NONE;

	throw IllegalStateException("both user and gateway are set as originators");
}

const User &Control::State::user() const
{
	if (m_user.isNull())
		throw IllegalStateException("no user originator defined");

	return m_user.value();
}

const Gateway &Control::State::gateway() const
{
	if (m_gateway.isNull())
		throw IllegalStateException("no gateway originator defined");

	return m_gateway.value();
}
