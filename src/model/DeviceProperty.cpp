#include <Poco/Exception.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/CipherKey.h>

#include "model/DeviceProperty.h"
#include "util/CryptoConfig.h"

using namespace std;
using namespace Poco;
using namespace Poco::Crypto;
using namespace Poco::Net;
using namespace BeeeOn;

EnumHelper<DevicePropertyKey::Raw>::ValueMap &DevicePropertyKeyEnum::valueMap()
{
	static EnumHelper<DevicePropertyKey::Raw>::ValueMap valueMap = {
		{DevicePropertyKey::KEY_INVALID, "invalid"},
		{DevicePropertyKey::KEY_IP_ADDRESS, "ip-address"},
		{DevicePropertyKey::KEY_PASSWORD, "password"},
		{DevicePropertyKey::KEY_FIRMWARE, "firmware"},
	};

	return valueMap;
}

DeviceProperty::DeviceProperty():
	m_key(DevicePropertyKey::fromRaw(DevicePropertyKey::KEY_INVALID))
{
}

void DeviceProperty::setKey(const DevicePropertyKey &key)
{
	m_key = key;
}

const DevicePropertyKey &DeviceProperty::key() const
{
	return m_key;
}

void DeviceProperty::setValue(const string &value)
{
	m_value = value;
}

string DeviceProperty::value() const
{
	return m_value;
}

void DeviceProperty::setIPAddress(const IPAddress &address, AutoPtr<Cipher> cipher)
{
	if (m_key != DevicePropertyKey::KEY_IP_ADDRESS)
		throw IllegalStateException("cannot set IP address for key " + m_key);

	if (m_params.empty())
		throw IllegalStateException("crypto params is empty for IP address");

	m_value = cipher->encryptString(address.toString(), Cipher::ENC_BASE64);
}

IPAddress DeviceProperty::asIPAddress(AutoPtr<Cipher> cipher) const
{
	if (m_key != DevicePropertyKey::KEY_IP_ADDRESS)
		throw IllegalStateException("cannot read IP address with key " + m_key);

	return IPAddress(cipher->decryptString(m_value, Cipher::ENC_BASE64));
}

void DeviceProperty::setPassword(const std::string &password, AutoPtr<Cipher> cipher)
{
	if (m_key != DevicePropertyKey::KEY_PASSWORD)
		throw IllegalStateException("cannot set password for key " + m_key);

	if (m_params.empty())
		throw IllegalStateException("crypto params is empty for password");

	m_value = cipher->encryptString(password, Cipher::ENC_BASE64);
}

string DeviceProperty::asPassword(AutoPtr<Cipher> cipher) const
{
	if (m_key != DevicePropertyKey::KEY_PASSWORD)
		throw IllegalStateException("cannot read password with key " + m_key);

	return cipher->decryptString(m_value, Cipher::ENC_BASE64);
}

void DeviceProperty::setFirmware(const string &firmware)
{
	if (m_key != DevicePropertyKey::KEY_FIRMWARE)
		throw IllegalStateException("cannot set firmware for key " + m_key);

	if (!m_params.empty())
		throw IllegalStateException("crypto params is not empty for firmware");

	m_value = firmware;
}

string DeviceProperty::asFirmware() const
{
	if (m_key != DevicePropertyKey::KEY_FIRMWARE)
		throw IllegalStateException("cannot read firmware with key " + m_key);

	return m_value;
}

void DeviceProperty::setParams(const CryptoParams &params)
{
	m_params = params;
}

CryptoParams DeviceProperty::params() const
{
	return m_params;
}

DecryptedDeviceProperty::DecryptedDeviceProperty(
		const DeviceProperty &property,
		const CryptoConfig *config):
	m_property(property)
{
	CipherFactory &factory = CipherFactory::defaultFactory();

	switch (property.key().raw()) {
	case DevicePropertyKey::KEY_IP_ADDRESS:
	case DevicePropertyKey::KEY_PASSWORD:
		if (config == NULL)
			throw IllegalStateException("no crypto configuration provided");

		m_cipher = factory.createCipher(config->createKey(property.params()));

	default:
		break;
	}
}

const DevicePropertyKey &DecryptedDeviceProperty::key() const
{
	return m_property.key();
}

IPAddress DecryptedDeviceProperty::asIPAddress() const
{
	return m_property.asIPAddress(m_cipher);
}

string DecryptedDeviceProperty::asPassword(bool placeholder) const
{
	if (placeholder)
		return "*****";
	else
		return m_property.asPassword(m_cipher);
}

string DecryptedDeviceProperty::asFirmware() const
{
	return m_property.asFirmware();
}
