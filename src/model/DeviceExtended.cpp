#include "model/DeviceExtended.h"

using namespace std;
using namespace BeeeOn;

DeviceExtended::DeviceExtended()
{
}

DeviceExtended::DeviceExtended(const Device &copy):
	DeviceWithData(copy)
{
}

DeviceExtended::DeviceExtended(const DeviceWithData &copy):
	DeviceWithData(copy)
{
}

DeviceExtended::DeviceExtended(const ID &id):
	DeviceWithData(id)
{
}

void DeviceExtended::setProperties(const list<DeviceProperty> &properties)
{
	m_properties = properties;
}

const list<DeviceProperty> DeviceExtended::properties() const
{
	return m_properties;
}
