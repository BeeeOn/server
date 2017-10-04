#include "model/Sensor.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

Sensor::Sensor()
{
}

Sensor::Sensor(const ID &id):
	AbstractModule(id)
{
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
