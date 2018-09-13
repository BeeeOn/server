#pragma once

#include <Poco/JSON/Object.h>

#include "service/Deserializer.h"
#include "model/Device.h"

namespace BeeeOn {

class JSONDeviceDeserializer : public Deserializer<Device> {
public:
	JSONDeviceDeserializer(const Poco::JSON::Object::Ptr object);

	void partial(Device &device) const override;
	void full(Device &device) const override;

protected:
	/**
	 * Refresh can be applied with certain rules only:
	 *
	 * - If the device proclaims that it does not support refresh
	 *   time changes, only a negative value is accepted.
	 *
	 * - If the device proclaims it supports refresh time changes,
	 *   only a non-negative value is accepted.
	 */
	void applyRefresh(Device &device, int refresh) const;

private:
	const Poco::JSON::Object::Ptr m_object;
};

}
