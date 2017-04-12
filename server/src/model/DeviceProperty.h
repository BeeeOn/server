#ifndef BEEEON_DEVICE_PROPERTY_H
#define BEEEON_DEVICE_PROPERTY_H

#include <string>

#include <Poco/Net/IPAddress.h>

#include "util/CryptoParams.h"
#include "util/Enum.h"

namespace Poco {
namespace Crypto {

class Cipher;

}
}

namespace BeeeOn {

class CryptoConfig;

struct DevicePropertyKeyEnum {
public:
	enum Raw {
		KEY_INVALID = 0,
		/**
		 * IP address where the associated device can be found.
		 * The password is encrypted to protect against database
		 * data stealing.
		 */
		KEY_IP_ADDRESS = 1,
		/**
		 * Password of the associated device. The password is
		 * encrypted to protect against database data stealing.
		 */
		KEY_PASSWORD = 2,
		/**
		 * Information about detected firmware in the target device.
		 */
		KEY_FIRMWARE = 3,
	};

	static EnumHelper<Raw>::ValueMap &valueMap();
};

typedef Enum<DevicePropertyKeyEnum> DevicePropertyKey;

/**
 * Every device can have a set of associated properties.
 * The properties are useful especially in case when a device
 * requires some special settings to be configured or found
 * properly in the PAN/LAN/WAN/...
 */
class DeviceProperty {
public:
	DeviceProperty();

	void setKey(const DevicePropertyKey &key);
	void setKey(const DevicePropertyKey::Raw &key)
	{
		setKey(DevicePropertyKey::fromRaw(key));
	}

	const DevicePropertyKey &key() const;

	void setValue(const std::string &value);
	std::string value() const;

	void setIPAddress(const Poco::Net::IPAddress &address, Poco::Crypto::Cipher *cipher);
	void setPassword(const std::string &password, Poco::Crypto::Cipher *cipher);
	void setFirmware(const std::string &firmware);

	Poco::Net::IPAddress asIPAddress(Poco::Crypto::Cipher *cipher) const;
	std::string asPassword(Poco::Crypto::Cipher *cipher) const;
	std::string asFirmware() const;

	void setParams(const CryptoParams &params);
	CryptoParams params() const;

private:
	DevicePropertyKey m_key;
	std::string m_value;
	CryptoParams m_params;
};

class DecryptedDeviceProperty {
public:
	DecryptedDeviceProperty(
		const DeviceProperty &property,
		const CryptoConfig *config);

	const DevicePropertyKey &key() const;
	Poco::Net::IPAddress asIPAddress() const;
	std::string asPassword(bool placeholder = true) const;
	std::string asFirmware() const;

private:
	const DeviceProperty &m_property;
	Poco::Crypto::Cipher *m_cipher;
};

}

#endif
