#include "model/Sensor.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

Sensor::Sensor()
{
}

Sensor::Sensor(const ID &id):
	Entity<SensorID>(id)
{
}

void Sensor::setInfo(const ModuleInfo &info)
{
	m_info = info;
}

const ModuleInfo &Sensor::info() const
{
	return m_info;
}

void Sensor::setName(const string &name)
{
	m_name = name;
}

string Sensor::name() const
{
	return m_name;
}

void Sensor::setValue(const double value)
{
	m_value = value;
}

double Sensor::value() const
{
	return m_value;
}

void Sensor::setAt(const Nullable<Timestamp> &at)
{
	m_at = at;
}

Nullable<Timestamp> Sensor::at() const
{
	return m_at;
}
