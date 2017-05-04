#include <Poco/UUIDGenerator.h>
#include <Poco/DateTime.h>
#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>
#include <Poco/Exception.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/CipherKey.h>

#include "di/Injectable.h"
#include "model/DeviceInfo.h"
#include "model/DeviceProperty.h"
#include "model/ModuleValue.h"
#include "dao/DeviceDao.h"
#include "dao/DevicePropertyDao.h"
#include "ui/UIMockInit.h"
#include "util/CryptoConfig.h"
#include "util/ValueGenerator.h"

BEEEON_OBJECT_BEGIN(BeeeOn, UIMockInit)
BEEEON_OBJECT_REF("userDao", &UIMockInit::setUserDao)
BEEEON_OBJECT_REF("gatewayDao", &UIMockInit::setGatewayDao)
BEEEON_OBJECT_REF("locationDao", &UIMockInit::setLocationDao)
BEEEON_OBJECT_REF("deviceDao", &UIMockInit::setDeviceDao)
BEEEON_OBJECT_REF("devicePropertyDao", &UIMockInit::setDevicePropertyDao)
BEEEON_OBJECT_REF("identityDao", &UIMockInit::setIdentityDao)
BEEEON_OBJECT_REF("verifiedIdentityDao", &UIMockInit::setVerifiedIdentityDao)
BEEEON_OBJECT_REF("deviceInfoProvider", &UIMockInit::setDeviceInfoProvider)
BEEEON_OBJECT_REF("sensorHistoryDao", &UIMockInit::setSensorHistoryDao)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_REF("cryptoConfig", &UIMockInit::setCryptoConfig)
BEEEON_OBJECT_HOOK("done", &UIMockInit::initAll)
BEEEON_OBJECT_END(BeeeOn, UIMockInit)

using namespace std;
using namespace Poco;
using namespace Poco::Crypto;
using namespace Poco::Net;
using namespace BeeeOn;

void UIMockInit::initUsers()
{
	User joeDoeUser(UserID::random());
	joeDoeUser.setFirstName("Joe");
	joeDoeUser.setLastName("Doe");

	m_userDao->create(joeDoeUser);

	Identity joeDoe0;
	joeDoe0.setEmail("joe.doe@example.org");

	m_identityDao->create(joeDoe0);

	VerifiedIdentity preparedJoeDoe0;
	preparedJoeDoe0.setIdentity(joeDoe0);
	preparedJoeDoe0.setUser(joeDoeUser);
	preparedJoeDoe0.setProvider("prepared");

	m_verifiedIdentityDao->create(preparedJoeDoe0);

	User johnsmithUser(UserID::random());
	johnsmithUser.setFirstName("John");
	johnsmithUser.setLastName("Smith");

	m_userDao->create(johnsmithUser);

	Identity johnsmith0;
	johnsmith0.setEmail("john.smith@example.org");

	m_identityDao->create(johnsmith0);

	VerifiedIdentity preparedJohnSmith0;
	preparedJohnSmith0.setIdentity(johnsmith0);
	preparedJohnSmith0.setUser(johnsmithUser);
	preparedJohnSmith0.setProvider("prepared");

	m_verifiedIdentityDao->create(preparedJohnSmith0);
}

void UIMockInit::initGateways()
{
	GatewayID id(GatewayID::parse("1284174504043136"));
	Gateway gateway(id);
	gateway.setName("Joe Doe's Gateway");

	m_gatewayDao->insert(gateway);

	GatewayID id2(GatewayID::parse("1780053541714013"));
	Gateway gateway2(id2);
	gateway2.setName("Gateway To Un/assign");

	m_gatewayDao->insert(gateway2);
}

void UIMockInit::initLocations(vector<Location> &locations)
{
	Gateway gateway(GatewayID::parse("1284174504043136"));

	if (!m_gatewayDao->fetch(gateway))
		throw IllegalStateException("no such gateway " + gateway.id().toString());

	Location kitchen(LocationID::random());
	kitchen.setName("Kitchen #1");
	kitchen.setGateway(gateway);

	m_locationDao->create(kitchen);
	locations.push_back(kitchen);

	Location livingroom(LocationID::random());
	livingroom.setName("Livingroom #1");
	livingroom.setGateway(gateway);

	m_locationDao->create(livingroom);
	locations.push_back(livingroom);

	Location bathroom(LocationID::random());
	bathroom.setName("Bathroom #1");
	bathroom.setGateway(gateway);

	m_locationDao->create(bathroom);
	locations.push_back(bathroom);

	Location hall(LocationID::random());
	hall.setName("Hall #1");
	hall.setGateway(gateway);

	m_locationDao->create(hall);
	locations.push_back(hall);
}

void UIMockInit::initDevices(const vector<Location> &locations)
{
	CipherFactory &factory = CipherFactory::defaultFactory();

	Gateway gateway(GatewayID::parse("1284174504043136"));

	Device temperature(DeviceID::parse("0x4135d00019f5234e"));
	temperature.setName("Temperature");
	temperature.setGateway(gateway);
	temperature.setLocation(locations[0]);
	temperature.setType(m_infoProvider->findById(0));
	temperature.setRefresh(5);
	temperature.setBattery(50.0);
	temperature.setSignal(90.0);
	temperature.setFirstSeen(DateTime(2015, 4, 9, 15, 43, 1));
	temperature.setLastSeen(DateTime(2016, 9, 1, 13, 27, 18));
	temperature.setActiveSince(DateTime(2015, 5, 2, 17, 59, 59));

	m_deviceDao->insert(temperature, gateway);

	DeviceProperty temperatureFirmware;
	temperatureFirmware.setKey(DevicePropertyKey::KEY_FIRMWARE);
	temperatureFirmware.setFirmware("v1.0-6453");

	m_devicePropertyDao->insert(temperatureFirmware, temperature);

	Device humidity(DeviceID::parse("0x427e0f7f0302324d"));
	humidity.setName("Humidity");
	humidity.setGateway(gateway);
	humidity.setLocation(locations[1]);
	humidity.setType(m_infoProvider->findById(0));
	humidity.setRefresh(1000);
	humidity.setBattery(99.0);
	humidity.setSignal(45.0);
	humidity.setFirstSeen(DateTime(2016, 8, 8, 8, 8, 8));
	humidity.setLastSeen(DateTime());
	humidity.setActiveSince(DateTime(2016, 8, 9, 8, 9, 8));

	m_deviceDao->insert(humidity, gateway);

	DeviceProperty humidityFirmware;
	humidityFirmware.setKey(DevicePropertyKey::KEY_FIRMWARE);
	humidityFirmware.setFirmware("000-111");

	m_devicePropertyDao->insert(humidityFirmware, humidity);

	Device multi(DeviceID::parse("0x432d27aa5e94ecfd"));
	multi.setName("Multi-sensor");
	multi.setGateway(gateway);
	multi.setLocation(locations[2]);
	multi.setType(m_infoProvider->findById(3));
	multi.setRefresh(15);
	multi.setBattery(90.0);
	multi.setSignal(90.0);
	multi.setFirstSeen(DateTime(2016, 9, 10, 11, 12, 13));
	multi.setLastSeen(DateTime(2016, 10, 10, 11, 11, 22));
	multi.setActiveSince(DateTime(2016, 9, 10, 11, 30, 1));

	m_deviceDao->insert(multi, gateway);

	DeviceProperty multiIPAddress;
	multiIPAddress.setKey(DevicePropertyKey::KEY_IP_ADDRESS);
	multiIPAddress.setParams(m_cryptoConfig->deriveParams());
	multiIPAddress.setIPAddress(IPAddress("10.0.0.1"),
			factory.createCipher(
				m_cryptoConfig->createKey(multiIPAddress.params())
			)
	);

	m_devicePropertyDao->insert(multiIPAddress, multi);

	DeviceProperty multiPassword;
	multiPassword.setKey(DevicePropertyKey::KEY_PASSWORD);
	multiPassword.setParams(m_cryptoConfig->deriveParams());
	multiPassword.setPassword("some secret password",
			factory.createCipher(
				m_cryptoConfig->createKey(multiIPAddress.params())
			)
	);

	m_devicePropertyDao->insert(multiPassword, multi);

	Device unknown(DeviceID::parse("0x4471959aad24618e"));
	unknown.setName("Unknown");
	unknown.setGateway(gateway);
	unknown.setType(m_infoProvider->findById(4));
	unknown.setRefresh(2);
	unknown.setFirstSeen(DateTime() - Timespan(100, 0));
	unknown.setLastSeen(DateTime() - Timespan(7, 0));

	m_deviceDao->insert(unknown, gateway);
}

void UIMockInit::initSensorData()
{
	Gateway gateway(GatewayID::parse("1284174504043136"));
	Device temperature(DeviceID::parse("0x4135d00019f5234e"));
	temperature.setGateway(gateway);

	DateTime now;

	ModuleInfoID inTempId(0);

	ConstGenerator inTempImpl(19.5);
	TimestampedGenerator inTempTimestamp(inTempImpl, now - Timespan(8, 0, 0, 0, 0), 260);
	TimeLimitedGenerator inTemp(inTempTimestamp, now);

	while (inTemp.hasNext()) {
		m_sensorHistoryDao->insert(
			temperature,
			inTemp.at(),
			ModuleValue(inTempId, inTemp.next())
		);
	}

	ModuleInfoID outTempId(1);

	SinGenerator outTempImpl;
	RangeGenerator outTempRange(outTempImpl, -25, 25);
	TimestampedGenerator outTempTimestamp(outTempRange, now - Timespan(0, 2, 0, 0, 0), 110);
	TimeLimitedGenerator outTemp(outTempTimestamp, now);

	while (outTemp.hasNext()) {
		m_sensorHistoryDao->insert(
			temperature,
			outTemp.at(),
			ModuleValue(outTempId, outTemp.next()));
	}

	ModuleInfoID humidityId(2);

	RandomGenerator humidityImpl;;
	RangeGenerator humidityRange(humidityImpl, 0, 100);
	TimestampedGenerator humidityTimestamp(humidityRange, now - Timespan(14, 0, 0, 0, 0), 720);
	TimeLimitedGenerator humidity(humidityTimestamp, now);

	while (humidity.hasNext()) {
		m_sensorHistoryDao->insert(
			temperature,
			humidity.at(),
			ModuleValue(humidityId, humidity.next()));
	}
}

void UIMockInit::initAll()
{
	vector<Location> locations;

	BEEEON_TRANSACTION(
		initUsers();
		initGateways();
		initLocations(locations);
		initDevices(locations);
		initSensorData();
	);
}
