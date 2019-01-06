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
		{DevicePropertyKey::KEY_MAC_ADDRESS, "mac-address"},
	};

	return valueMap;
}

bool DevicePropertyKeyEnum::isUserWritable() const
{
	switch (static_cast<const DevicePropertyKey &>(*this).raw()) {
	case KEY_IP_ADDRESS:
		return true;
	case KEY_PASSWORD:
		return true;
	case KEY_FIRMWARE:
		return false;
	case KEY_MAC_ADDRESS:
		return false;
	case KEY_INVALID:
		break;
	}

	return false;
}

bool DevicePropertyKeyEnum::isUserReadable() const
{
	switch (static_cast<const DevicePropertyKey &>(*this).raw()) {
	case KEY_IP_ADDRESS:
		return true;
	case KEY_PASSWORD:
		return false;
	case KEY_FIRMWARE:
		return true;
	case KEY_MAC_ADDRESS:
		return true;
	case KEY_INVALID:
		break;
	}

	return false;
}

bool DevicePropertyKeyEnum::isGatewayWritable() const
{
	switch (static_cast<const DevicePropertyKey &>(*this).raw()) {
	case KEY_IP_ADDRESS:
		return true;
	case KEY_PASSWORD:
		return false;
	case KEY_FIRMWARE:
		return true;
	case KEY_MAC_ADDRESS:
		return true;
	case KEY_INVALID:
		break;
	}

	return false;
}

bool DevicePropertyKeyEnum::isGatewayReadable() const
{
	switch (static_cast<const DevicePropertyKey &>(*this).raw()) {
	case KEY_IP_ADDRESS:
		return true;
	case KEY_PASSWORD:
		return true;
	case KEY_FIRMWARE:
		return true;
	case KEY_MAC_ADDRESS:
		return true;
	case KEY_INVALID:
		break;
	}

	return false;
}

CryptoParams DevicePropertyKeyEnum::deriveParams(const CryptoConfig &config) const
{
	const auto &key = static_cast<const DevicePropertyKey &>(*this);
	switch (key.raw()) {
	case KEY_IP_ADDRESS:
		return config.deriveParams();
	case KEY_PASSWORD:
		return config.deriveParams();
	case KEY_FIRMWARE:
		return CryptoParams::createEmpty();
	case KEY_MAC_ADDRESS:
		return config.deriveParams();
	case KEY_INVALID:
		return CryptoParams::createEmpty();
	}

	throw IllegalStateException(
		"cannot create crypto params for property key '"
		+ key.toString() + "'");
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

void DeviceProperty::setMACAddress(const MACAddress &address, AutoPtr<Cipher> cipher)
{
	if (m_key != DevicePropertyKey::KEY_MAC_ADDRESS)
		throw IllegalStateException("cannot set MAC address for key " + m_key);

	if (m_params.empty())
		throw IllegalStateException("crypto params is empty for MAC address");

	m_value = cipher->encryptString(address.toString(), Cipher::ENC_BASE64);
}

MACAddress DeviceProperty::asMACAddress(AutoPtr<Cipher> cipher) const
{
	if (m_key != DevicePropertyKey::KEY_MAC_ADDRESS)
		throw IllegalStateException("cannot read MAC address with key " + m_key);

	return MACAddress::parse(cipher->decryptString(m_value, Cipher::ENC_BASE64));
}


void DeviceProperty::setFromString(
		const string &input,
		const CryptoConfig &config)
{
	CipherFactory &factory = CipherFactory::defaultFactory();
	AutoPtr<Cipher> cipher;

	switch (key().raw()) {
	case DevicePropertyKey::KEY_IP_ADDRESS:
		cipher = factory.createCipher(config.createKey(params()));
		setIPAddress(IPAddress::parse(input), cipher);
		break;

	case DevicePropertyKey::KEY_PASSWORD:
		cipher = factory.createCipher(config.createKey(params()));
		setPassword(input, cipher);
		break;

	case DevicePropertyKey::KEY_FIRMWARE:
		setFirmware(input);
		break;

	case DevicePropertyKey::KEY_MAC_ADDRESS:
		cipher = factory.createCipher(config.createKey(params()));
		setMACAddress(MACAddress::parse(input), cipher);
		break;

	case DevicePropertyKey::KEY_INVALID:
		throw InvalidArgumentException(
			"device property '" + key().toString() + "' cannot be parsed");

	}
}

string DeviceProperty::asString(AutoPtr<Cipher> cipher) const
{
	switch (m_key) {
	case DevicePropertyKey::KEY_IP_ADDRESS:
		return asIPAddress(cipher).toString();
	case DevicePropertyKey::KEY_PASSWORD:
		return asPassword(cipher);
	case DevicePropertyKey::KEY_FIRMWARE:
		return asFirmware();
	case DevicePropertyKey::KEY_MAC_ADDRESS:
		return asMACAddress(cipher).toString(':');
	case DevicePropertyKey::KEY_INVALID:
		break;
	}

	throw IllegalStateException("unable to stringize property " + m_key.toString());
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
	case DevicePropertyKey::KEY_MAC_ADDRESS:
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

string DecryptedDeviceProperty::asPassword() const
{
	return m_property.asPassword(m_cipher);
}

string DecryptedDeviceProperty::asFirmware() const
{
	return m_property.asFirmware();
}

MACAddress DecryptedDeviceProperty::asMACAddress() const
{
	return m_property.asMACAddress(m_cipher);
}

string DecryptedDeviceProperty::asString() const
{
	return m_property.asString(m_cipher);
}
