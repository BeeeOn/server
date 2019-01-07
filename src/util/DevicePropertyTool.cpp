#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "model/Device.h"
#include "model/DeviceProperty.h"
#include "util/ArgsParser.h"
#include "util/DevicePropertyTool.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DevicePropertyTool)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("cryptoConfig", &DevicePropertyTool::setCryptoConfig)
BEEEON_OBJECT_PROPERTY("devicePropertyDao", &DevicePropertyTool::setDevicePropertyDao)
BEEEON_OBJECT_PROPERTY("transactionManager", &DevicePropertyTool::setTransactionManager)
BEEEON_OBJECT_PROPERTY("cmd", &DevicePropertyTool::setCommand)
BEEEON_OBJECT_PROPERTY("console", &DevicePropertyTool::setConsole)
BEEEON_OBJECT_END(BeeeOn, DevicePropertyTool)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DevicePropertyTool::DevicePropertyTool()
{
}

void DevicePropertyTool::setDevicePropertyDao(DevicePropertyDao::Ptr dao)
{
	m_propertyDao = dao;
}

void DevicePropertyTool::setCryptoConfig(SharedPtr<CryptoConfig> config)
{
	m_cryptoConfig = config;
}

void DevicePropertyTool::main(
		ConsoleSession &session,
		const vector<string> &args)
{
	if (!command().empty()) {
		processLine(command(), session);
		return;
	}

	bool stop = false;

	while (!stop && !shouldStop()) {
		if (session.eof())
			break;

		string line = session.readLine();
		trimInPlace(line);

		if (line.empty())
			continue;

		try {
			stop = processLine(line, session);
		}
		catch (const Exception &e) {
			session.print(e.displayText());
		}
	}
}

bool DevicePropertyTool::processLine(
		const string &line,
		ConsoleSession &session)
{
	ArgsParser parser;
	vector<string> args;

	try {
		args = parser.parse(line);
	}
	catch (const Exception &e) {
		session.print(e.displayText());
		return false;
	}

	if (args.empty())
		return false;

	if (args[0] == "list")
		actionList(session, {++begin(args), end(args)});
	else if (args[0] == "create")
		actionCreate(session, {++begin(args), end(args)});
	else if (args[0] == "remove")
		actionRemove(session, {++begin(args), end(args)});
	else if (args[0] == "exit")
		return true;
	else
		throw InvalidArgumentException("unrecognized command: " + args[0]);

	return false;
}

void DevicePropertyTool::actionList(
		ConsoleSession &session,
		const vector<string> &args)
{
	if (args.size() < 2)
		throw InvalidArgumentException("missing at least 2 arguments");

	Device device(DeviceID::parse(args[1]));
	device.setGateway({GatewayID::parse(args[0])});

	list<DeviceProperty> properties;

	BEEEON_TRANSACTION(m_propertyDao->fetchByDevice(properties, device));

	for (const auto &property : properties) {
		session.print(property.key().toString() + ": ", false);

		try {
			DecryptedDeviceProperty decrypted(property, m_cryptoConfig);
			session.print(decrypted.asString());
		}
		BEEEON_CATCH_CHAIN_ACTION(logger(),
			session.print("<error>"))
	}
}

void DevicePropertyTool::actionCreate(
		ConsoleSession &session,
		const vector<string> &args)
{
	if (args.size() < 4)
		throw InvalidArgumentException("missing at least 4 arguments");

	Device device(DeviceID::parse(args[1]));
	device.setGateway({GatewayID::parse(args[0])});

	const auto key = DevicePropertyKey::parse(args[2]);
	if (key == DevicePropertyKey::KEY_INVALID) {
		throw InvalidArgumentException(
			"cannot create property of key " + key.toString());
	}

	const auto value = args[3];

	DeviceProperty property;
	property.setKey(key);
	property.setParams(key.deriveParams(*m_cryptoConfig));
	property.setFromString(value, *m_cryptoConfig);

	BEEEON_TRANSACTION(
		DeviceProperty tmp;
		tmp.setKey(key);

		if (m_propertyDao->fetch(tmp, device)) {
			throw IllegalStateException(
				"property " + property.key() + " already exists");
		}

		m_propertyDao->insert(property, device));
}

void DevicePropertyTool::actionRemove(
		ConsoleSession &session,
		const vector<string> &args)
{
	if (args.size() < 3)
		throw InvalidArgumentException("missing at least 3 arguments");

	Device device(DeviceID::parse(args[1]));
	device.setGateway({GatewayID::parse(args[0])});

	const auto key = DevicePropertyKey::parse(args[2]);
	if (key == DevicePropertyKey::KEY_INVALID) {
		throw InvalidArgumentException(
			"cannot create property of key " + key.toString());
	}

	DeviceProperty property;
	property.setKey(key);

	BEEEON_TRANSACTION(
		if (!m_propertyDao->fetch(property, device))
			return;

		m_propertyDao->remove(property, device));
}
