#pragma once

#include <list>

#include "model/DeviceProperty.h"
#include "model/DeviceWithData.h"

namespace BeeeOn {

class DeviceExtended : public DeviceWithData {
public:
	DeviceExtended();
	DeviceExtended(const Device &copy);
	DeviceExtended(const DeviceWithData &copy);
	DeviceExtended(const ID &id);

	void setProperties(const std::list<DeviceProperty> &properties);
	const std::list<DeviceProperty> properties() const;

private:
	std::list<DeviceProperty> m_properties;
};

}
