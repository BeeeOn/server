#include <Poco/AutoPtr.h>
#include <Poco/Environment.h>
#include <Poco/Exception.h>
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Net/IPAddress.h>

#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "model/DeviceProperty.h"

using namespace std;
using namespace Poco;
using namespace Poco::Crypto;
using namespace Poco::Net;

namespace BeeeOn {

class DevicePropertyTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DevicePropertyTest);
	CPPUNIT_TEST(testNoFirmware);
	CPPUNIT_TEST(testFirmware);
	CPPUNIT_TEST(testNoIPAddress);
	CPPUNIT_TEST(testIPAddress);
	CPPUNIT_TEST(testNoMACAddress);
	CPPUNIT_TEST(testMACAddress);
	CPPUNIT_TEST(testNoPassword);
	CPPUNIT_TEST(testPassword);
	CPPUNIT_TEST_SUITE_END();

public:
	DevicePropertyTest();

	void setUp() override;

	void testNoFirmware();
	void testFirmware();
	void testNoIPAddress();
	void testIPAddress();
	void testNoMACAddress();
	void testMACAddress();
	void testNoPassword();
	void testPassword();

private:
	CipherFactory &m_factory;
	string m_cipherName;
	CryptoParams m_params;
	DeviceProperty m_property;
};

CPPUNIT_TEST_SUITE_REGISTRATION(DevicePropertyTest);

DevicePropertyTest::DevicePropertyTest():
	m_factory(CipherFactory::defaultFactory())
{
}

void DevicePropertyTest::setUp()
{
	m_cipherName = Environment::get("TEST_CIPHER_NAME", "aes256");

	m_params = CryptoParams::create(m_cipherName);

	m_property.setKey(DevicePropertyKey::KEY_INVALID);
	m_property.setParams(CryptoParams::createEmpty());
}

/**
 * Test that when working with firmware the DevicePropertyKey::KEY_FIRMWARE
 * must be set properly.
 */
void DevicePropertyTest::testNoFirmware()
{
	CPPUNIT_ASSERT_THROW(
		m_property.setFirmware("super version"),
		Poco::IllegalStateException
	);
	CPPUNIT_ASSERT_THROW(
		m_property.asFirmware(),
		Poco::IllegalStateException
	);
}

/**
 * Test firmware information can be set and read properly.
 */
void DevicePropertyTest::testFirmware()
{
	m_property.setKey(DevicePropertyKey::KEY_FIRMWARE);
	m_property.setFirmware("super version");

	CPPUNIT_ASSERT_EQUAL(
		"super version",
		m_property.asFirmware()
	);

	CPPUNIT_ASSERT_EQUAL("super version", m_property.value());
}

/**
 * Test that when working with IP Address the DevicePropertyKey::KEY_IP_ADDRESS
 * must be set properly.
 */
void DevicePropertyTest::testNoIPAddress()
{
	const CipherKey &key = m_params.randomKey();
	AutoPtr<Cipher> cipher = m_factory.createCipher(key);

	m_property.setParams(m_params);

	CPPUNIT_ASSERT_THROW(
		m_property.setIPAddress(IPAddress("192.168.0.1"), cipher),
		Poco::IllegalStateException
	);
	CPPUNIT_ASSERT_THROW(
		m_property.asIPAddress(cipher),
		Poco::IllegalStateException
	);
}

/**
 * Test IP address (v4 or v6) can be set and read properly.
 */
void DevicePropertyTest::testIPAddress()
{
	const CipherKey &key = m_params.randomKey();
	AutoPtr<Cipher> cipher = m_factory.createCipher(key);

	m_property.setParams(m_params);
	m_property.setKey(DevicePropertyKey::KEY_IP_ADDRESS);

	m_property.setIPAddress(IPAddress("192.168.0.1"), cipher);

	CPPUNIT_ASSERT_EQUAL(
		"192.168.0.1",
		m_property.asIPAddress(cipher).toString()
	);

	CPPUNIT_ASSERT(m_property.value() != "192.168.0.1");

	m_property.setIPAddress(IPAddress("fdd7:b085:62e0::567:112:abc"), cipher);

	CPPUNIT_ASSERT_EQUAL(
		"fdd7:b085:62e0::567:112:abc",
		m_property.asIPAddress(cipher).toString()
	);

	CPPUNIT_ASSERT(m_property.value() != "fdd7:b085:62e0::567:112:abc");
}

/**
 * Test that when working with MAC Address the DevicePropertyKey::KEY_MAC_ADDRESS
 * must be set properly.
 */
void DevicePropertyTest::testNoMACAddress()
{
	const CipherKey &key = m_params.randomKey();
	AutoPtr<Cipher> cipher = m_factory.createCipher(key);

	m_property.setParams(m_params);

	CPPUNIT_ASSERT_THROW(
		m_property.setMACAddress(MACAddress::parse("11:22:33:44:55:66"), cipher),
		Poco::IllegalStateException
	);
	CPPUNIT_ASSERT_THROW(
		m_property.asMACAddress(cipher),
		Poco::IllegalStateException
	);
}

/**
 * Test MAC address can be set and read properly.
 */
void DevicePropertyTest::testMACAddress()
{
	const CipherKey &key = m_params.randomKey();
	AutoPtr<Cipher> cipher = m_factory.createCipher(key);

	m_property.setParams(m_params);
	m_property.setKey(DevicePropertyKey::KEY_MAC_ADDRESS);

	m_property.setMACAddress(MACAddress::parse("11:22:33:44:55:66"), cipher);

	CPPUNIT_ASSERT_EQUAL(
		"11:22:33:44:55:66",
		m_property.asMACAddress(cipher).toString(':')
	);

	CPPUNIT_ASSERT(m_property.value() != "11:22:33:44:55:66");
}

/**
 * Test that when working with password the DevicePropertyKey::KEY_PASSWORD
 * must be set properly.
 */
void DevicePropertyTest::testNoPassword()
{
	const CipherKey &key = m_params.randomKey();
	AutoPtr<Cipher> cipher = m_factory.createCipher(key);

	m_property.setParams(m_params);

	CPPUNIT_ASSERT_THROW(
		m_property.setPassword("secret text", cipher),
		Poco::IllegalStateException
	);
	CPPUNIT_ASSERT_THROW(
		m_property.asPassword(cipher),
		Poco::IllegalStateException
	);
}

/**
 * Test password can be set and read with encryption on-fly properly.
 */
void DevicePropertyTest::testPassword()
{
	const CipherKey &key = m_params.randomKey();
	AutoPtr<Cipher> cipher = m_factory.createCipher(key);

	m_property.setParams(m_params);
	m_property.setKey(DevicePropertyKey::KEY_PASSWORD);
	m_property.setPassword("secret text", cipher);

	CPPUNIT_ASSERT_EQUAL(
		"secret text",
		m_property.asPassword(cipher)
	);

	CPPUNIT_ASSERT(m_property.value() != "secret text");
}

}
