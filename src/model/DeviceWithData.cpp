#include "model/DeviceWithData.h"

using namespace std;
using namespace BeeeOn;

DeviceWithData::DeviceWithData()
{
}

DeviceWithData::DeviceWithData(const Device &copy):
	Device(copy)
{
}

DeviceWithData::DeviceWithData(const ID &id):
	Device(id)
{
}

void DeviceWithData::setValues(const vector<ValueAt> &values)
{
	m_values = values;
}

const vector<ValueAt> DeviceWithData::values() const
{
	return m_values;
}
