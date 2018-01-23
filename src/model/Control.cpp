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

Control::RequestedValue::RequestedValue():
	m_value(NAN),
	m_result(RESULT_UNKNOWN)
{
}

Control::RequestedValue::RequestedValue(
		const User &originator,
		const Timestamp &at,
		double value):
	m_originator(originator),
	m_value(value),
	m_result(RESULT_UNKNOWN),
	m_requestedAt(at)
{
}

void Control::RequestedValue::setValue(double value)
{
	m_value = value;
}

double Control::RequestedValue::value() const
{
	return m_value;
}

bool Control::RequestedValue::isValueValid() const
{
	return !std::isnan(m_value);
}

bool Control::RequestedValue::hasStarted() const
{
	return !m_requestedAt.isNull();
}

bool Control::RequestedValue::isActive() const
{
	return hasStarted() && m_finishedAt.isNull();
}

bool Control::RequestedValue::hasFailed() const
{
	return hasStarted() && failed();
}

void Control::RequestedValue::setResult(const Result &result)
{
	m_result = result;
}

Control::RequestedValue::Result Control::RequestedValue::result() const
{
	return m_result;
}

void Control::RequestedValue::setOriginator(const User &originator)
{
	m_originator = originator;
}

const User &Control::RequestedValue::originator() const
{
	return m_originator;
}

void Control::RequestedValue::setRequestedAt(const Nullable<Timestamp> &at)
{
	m_requestedAt = at;
}

Nullable<Timestamp> Control::RequestedValue::requestedAt() const
{
	return m_requestedAt;
}

void Control::RequestedValue::setAcceptedAt(const Nullable<Timestamp> &at)
{
	m_acceptedAt = at;
}

Nullable<Timestamp> Control::RequestedValue::acceptedAt() const
{
	return m_acceptedAt;
}

void Control::RequestedValue::setFinishedAt(const Nullable<Timestamp> &at)
{
	m_finishedAt = at;
}

Nullable<Timestamp> Control::RequestedValue::finishedAt() const
{
	return m_finishedAt;
}

void Control::setRecentValue(const ValueAt &value)
{
	m_recentValue = value;
}

const ValueAt &Control::recentValue() const
{
	return m_recentValue;
}

void Control::setRequestedValue(const RequestedValue &value)
{
	m_requestedValue = value;
}

const Control::RequestedValue &Control::requestedValue() const
{
	return m_requestedValue;
}
