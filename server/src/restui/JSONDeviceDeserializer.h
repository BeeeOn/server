#ifndef BEEEON_JSON_DEVICE_DESERIALIZER_H
#define BEEEON_JSON_DEVICE_DESERIALIZER_H

#include <Poco/JSON/Object.h>

#include "service/Deserializer.h"
#include "model/Device.h"

namespace BeeeOn {

class JSONDeviceDeserializer : public Deserializer<Device> {
public:
	JSONDeviceDeserializer(const Poco::JSON::Object::Ptr object);

	void partial(Device &device) const override;
	void full(Device &device) const override;

private:
	const Poco::JSON::Object::Ptr m_object;
};

}

#endif
