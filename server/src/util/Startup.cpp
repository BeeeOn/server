#include <iostream>

#include <Poco/Version.h>
#include <Poco/Logger.h>
#include <Poco/String.h>
#include <Poco/Message.h>
#include <Poco/Environment.h>
#include <Poco/Exception.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/HelpFormatter.h>

#include "util/Startup.h"
#include "util/PosixSignal.h"
#include "util/PocoVersion.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace BeeeOn;

static Option optConfig("config", "c",
		"general configuration file to be used (xml, ini, properties)",
		false,
		"<file>",
		true);
static Option optServices("services", "s",
		"services configuration file to be used (xml, ini, properties)",
		false,
		"<file>",
		true);
static Option optLogging("logging", "l",
		"logging configuration file to be used (xml, ini, properties)",
		false,
		"<file>",
		true);
static Option optLogLevel("log-level", "L",
		"change startup log-level (trace, debug, [info],"
		" notice, warn, error, fatal, trace)",
		false,
		"<level>",
		true);
static Option optPort("port", "p",
		"server port to listen on",
		false,
		"<port>",
		true);
static Option optNotifyStarted("notify-started", "N",
		"notify process given as PID by sending it SIGINT when the application started up",
		false,
		"<PID>",
		true);
static Option optDefine("define", "D",
		"define configuration options (to override configuration files)",
		false,
		"<key>=<value>",
		true);
static Option optHelp("help", "h", "print help");

int ServerStartup::parseLogLevel(const string &level)
{
	if (icompare(level, "info") == 0)
		return Message::PRIO_INFORMATION;
	if (icompare(level, "warn") == 0)
		return Message::PRIO_WARNING;
	if (icompare(level, "err") == 0)
		return Message::PRIO_ERROR;
	if (icompare(level, "crit") == 0)
		return Message::PRIO_CRITICAL;

	return Logger::parseLevel(level);
}

void ServerStartup::overrideConfig(const string text)
{
	StringTokenizer tokenizer(text, "=", StringTokenizer::TOK_TRIM);
	if (tokenizer.count() != 2)
		throw InvalidArgumentException("-D option requires a <key>=<value> pair");

	logger().debug("overriding " + tokenizer[0] + " = " + tokenizer[1],
			__FILE__, __LINE__);
	config().setString(tokenizer[0], tokenizer[1]);
}

void ServerStartup::handleOption(const string &name, const string &value)
{
	if (name == "services")
		m_userServices = value;
	if (name == "logging")
		m_userLogging = value;
	if (name == "config")
		m_userConfig = value;
	if (name == "help")
		m_printHelp = true;
	if (name == "log-level")
		logger().setLevel(parseLogLevel(value));
	if (name == "port")
		m_serverPort = stoi(value);
	if (name == "define")
		overrideConfig(value);
	if (name == "notify-started")
		m_notifyPid = stoi(value);

	if (m_printHelp)
		stopOptionsProcessing();

	Application::handleOption(name, value);
}

void ServerStartup::loadAllConfiguration()
{
	findAndLoadConfig();
	findAndLoadLogging();
	findAndLoadServices();
}

string ServerStartup::parentPath(const File &file)
{
	Path path(file.path());
	return path.parent().toString();
}

bool ServerStartup::readConfiguration(const File &file)
{
	if (!file.exists()) {
		logger().debug("configuration file "
				+ file.path() + " not found");
		return false;
	}

	loadConfiguration(file.path());
	logger().notice("configuration file "
			+ file.path() + " loaded");
	return true;
}

void ServerStartup::findAndLoadConfig()
{
	File user(m_userConfig);
	File system(defaultConfigFile());

	config().setString("config.dir",
		config().getString("application.dir"));

	if (!m_userConfig.empty() && readConfiguration(user))
		config().setString("config.dir", parentPath(user));
	else if (readConfiguration(system))
		config().setString("config.dir", parentPath(system));
	else
		logger().warning("no main configration found");
}

void ServerStartup::findAndLoadLogging()
{
	File user(m_userLogging);
	File system(config().getString(
			"ui.config.logging", defaultLoggingFile()));

	if (!m_userLogging.empty() && readConfiguration(user))
		return;
	else if (!readConfiguration(system.path()))
		logger().warning("no logging configuration found");
}

void ServerStartup::findAndLoadServices()
{
	File user(m_userServices);
	File system(config().getString(
			"ui.config.services", defaultServicesFile()));

	if (!m_userServices.empty() && readConfiguration(user))
		return;
	else if (!readConfiguration(system))
		logger().warning("no services configuration found");
}

void ServerStartup::initialize(Application &app)
{
	if (m_printHelp)
		return;

	loadAllConfiguration();
	Application::initialize(app);
}

void ServerStartup::defineOptions(OptionSet &options)
{
	options.addOption(optConfig);
	options.addOption(optServices);
	options.addOption(optLogging);
	options.addOption(optLogLevel);
	options.addOption(optPort);
	options.addOption(optNotifyStarted);
	optDefine.repeatable(true);
	options.addOption(optDefine);
	options.addOption(optHelp);
	Application::defineOptions(options);
}

int ServerStartup::printHelp()
{
	HelpFormatter help(options());
	help.setCommand(config().getString("application.baseName"));
	help.setUnixStyle(true);
	help.setWidth(80);
	help.setUsage("[-h] ...");
	help.format(cout);

	return 0;
}

string ServerStartup::defaultLoggingFile() const
{
	string path("/etc");
	return path + "/" + m_appGroup + "/" + m_appName + "/logging.ini";
}

string ServerStartup::defaultConfigFile() const
{
	string path("/etc");
	return path + "/" + m_appGroup + "/" + m_appName + "/server.ini";
}

string ServerStartup::defaultServicesFile() const
{
	string path("/etc");
	return path + "/" + m_appGroup + "/" + m_appName + "/services.xml";
}

static string pocoVersion(unsigned long version = 0)
{
	version = version == 0? Environment::libraryVersion() : version;

	unsigned int major = (version >> 24) & 0xff;
	unsigned int minor = (version >> 16) & 0xff;
	unsigned int alpha = (version >>  8) & 0xff;
	unsigned int  beta = (version >>  0) & 0xff;

	return to_string(major) + "." + to_string(minor) + "." + to_string(alpha) + "-" + to_string(beta);
}

static string pocoLinkedVersion(void)
{
	return pocoVersion(POCO_VERSION);
}

static void warnAboutPocoVersion(Logger &logger)
{
	bool upgrade = false;

	if (Environment::libraryVersion() > POCO_VERSION)
		logger.warning("runtime Poco library is newer than built-in headers", __FILE__, __LINE__);

	if (POCO_VERSION < RECOMMENDED_POCO_VERSION) {
		logger.warning("Poco library headers are older then recommended",
				__FILE__, __LINE__);
		upgrade = true;
	}

	if (Environment::libraryVersion() < RECOMMENDED_POCO_VERSION) {
		logger.warning("runtime Poco library is older then recommended",
				__FILE__, __LINE__);
		upgrade = true;
	}

	if (upgrade) {
		logger.warning("recommended to upgrade to Poco library "
				+ pocoVersion(RECOMMENDED_POCO_VERSION) + " or newer",
				__FILE__, __LINE__);
	}
}

void ServerStartup::notifyStarted()
{
	if (m_notifyPid >= 0) {
		try {
			PosixSignal::send(m_notifyPid, "SIGTERM");
			logger().debug("started, notify process " + to_string(m_notifyPid),
					__FILE__, __LINE__);
		} catch (const Exception &e) {
			logger().log(e, __FILE__, __LINE__);
		}
	}
}

int ServerStartup::main(const vector<string> &args)
{
	logger().notice("Poco library " + pocoVersion() + " (headers " + pocoLinkedVersion() + ")");
	warnAboutPocoVersion(logger());

	logger().notice("OS " + Environment::osDisplayName()
		+ " (" + Environment::osName() + " " + Environment::osVersion() + ")");
	logger().notice("Machine " + Environment::osArchitecture() + " (cores: " + to_string(Environment::processorCount()) + ")");
	logger().debug("Node " +  Environment::nodeName() + " (" + Environment::nodeId() + ")");

	if (m_printHelp)
		return printHelp();

	return execute();
}


int BeeeOn::generic_main(int argc, char **argv, ServerApplication &app)
{
	try {
		poco_throw_on_signal;

		app.setUnixOptions(true);
		return app.run(argc, argv);
	} catch(Exception &e) {
		cerr << e.displayText() << endl;
	} catch(exception &e) {
		cerr << e.what() << endl;
	} catch(const char *s) {
		cerr << s << endl;
	}

	return -1;
}
