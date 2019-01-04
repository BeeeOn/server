#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "model/DeviceProperty.h"
#include "service/Deserializer.h"
#include "util/CryptoConfig.h"

namespace BeeeOn {

class JSONDevicePropertyDeserializer : public Deserializer<DeviceProperty> {
public:
	JSONDevicePropertyDeserializer(
		const Poco::JSON::Object::Ptr object,
		const Poco::SharedPtr<CryptoConfig> config);

	void partial(DeviceProperty &property) const override;
	void full(DeviceProperty &property) const override;

protected:
	void assureWritable(const DeviceProperty &property) const;
	void deserialize(DeviceProperty &property) const;

private:
	const Poco::JSON::Object::Ptr m_object;
	Poco::SharedPtr<CryptoConfig> m_cryptoConfig;
};

}
