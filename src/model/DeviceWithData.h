#pragma once

#include <vector>

#include "model/Device.h"
#include "model/ValueAt.h"

namespace BeeeOn {

/**
 * @brief Represents a Device with the current values of all its modules.
 * The included values are related to the Device's timestamp last-seen
 * in the way that values' timestamps are always older or same as the
 * last-seen timestamp.
 */
class DeviceWithData : public Device {
public:
	DeviceWithData();
	DeviceWithData(const Device &copy);
	DeviceWithData(const ID &id);

	void setValues(const std::vector<ValueAt> &values);
	const std::vector<ValueAt> values() const;

private:
	std::vector<ValueAt> m_values;
};

}
