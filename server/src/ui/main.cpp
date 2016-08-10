#include <iostream>
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <Poco/File.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/ServerApplication.h>

#include "server/MongooseServer.h"
#include "di/DependencyInjector.h"
#include "UIServerModule.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace BeeeOn;

#define DEFAULT_PORT 8000

#define SYSTEM_LOGGING_FILE "/etc/beeeon/ui-server/logging.ini"
#define SYSTEM_CONFIG_FILE "/etc/beeeon/ui-server/server.ini"
#define SYSTEM_SERVICES_FILE "/etc/beeeon/ui-server/services.xml"

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
static Option optPort("port", "p",
		"server port to listen on",
		false,
		"<port>",
		true);
static Option optHelp("help", "h", "print help");

class Startup : public ServerApplication {
public:
	Startup():
		m_printHelp(false),
		m_serverPort(DEFAULT_PORT)
	{
	}

protected:
	void handleOption(const string &name, const string &value)
	{
		if (name == "services")
			m_userServices = value;
		if (name == "logging")
			m_userLogging = value;
		if (name == "config")
			m_userConfig = value;
		if (name == "help")
			m_printHelp = true;
		if (name == "port")
			m_serverPort = stoi(value);

		Application::handleOption(name, value);
	}

	void loadAllConfiguration()
	{
		findAndLoadConfig();
		findAndLoadLogging();
		findAndLoadServices();
	}

	string parentPath(const File &file)
	{
		Path path(file.path());
		return path.parent().toString();
	}

	bool readConfiguration(const File &file)
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

	void findAndLoadConfig()
	{
		File user(m_userConfig);
		File system(SYSTEM_CONFIG_FILE);

		config().setString("config.dir",
			config().getString("application.dir"));

		if (!m_userConfig.empty() && readConfiguration(user))
			config().setString("config.dir", parentPath(user));
		else if (readConfiguration(system))
			config().setString("config.dir", parentPath(system));
		else
			logger().warning("no main configration found");
	}

	void findAndLoadLogging()
	{
		File user(m_userLogging);
		File system(config().getString(
				"ui.config.logging", SYSTEM_LOGGING_FILE));

		if (!m_userLogging.empty() && readConfiguration(user))
			return;
		else if (!readConfiguration(system.path()))
			logger().warning("no logging configuration found");
	}

	void findAndLoadServices()
	{
		File user(m_userServices);
		File system(config().getString(
				"ui.config.services", SYSTEM_SERVICES_FILE));

		if (!m_userServices.empty() && readConfiguration(user))
			return;
		else if (!readConfiguration(system))
			logger().warning("no services configuration found");
	}

	void initialize(Application &app)
	{
		Logger::root().setLevel(Logger::parseLevel("trace"));
		loadAllConfiguration();
		Application::initialize(app);
	}

	void defineOptions(OptionSet &options)
	{
		options.addOption(optConfig);
		options.addOption(optServices);
		options.addOption(optLogging);
		options.addOption(optPort);
		options.addOption(optHelp);
		Application::defineOptions(options);
	}

	int printHelp()
	{
		HelpFormatter help(options());
		help.setCommand(config().getString("application.baseName"));
		help.setUnixStyle(true);
		help.setWidth(80);
		help.setUsage("[-h] ...");
		help.format(cout);

		return EXIT_OK;
	}

	int main(const std::vector <std::string> &args)
	{
		if (m_printHelp)
			return printHelp();

		if (logger().debug())
			ManifestSingleton::reportInfo(logger());

		DependencyInjector injector(config().createView("services"));
		UIServerModule *module = injector
					.create<UIServerModule>("main");

		module->createServer(m_serverPort);
		module->server().start();

		waitForTerminationRequest();
		module->server().stop();
		return EXIT_OK;
	}

private:
	bool m_printHelp;
	unsigned int m_serverPort;
	string m_userLogging;
	string m_userConfig;
	string m_userServices;
};

int main(int argc, char **argv)
{
	Startup server;
	server.setUnixOptions(true);

	try {
		return server.run(argc, argv);
	} catch(Exception &e) {
		cerr << e.displayText() << endl;
	} catch(exception &e) {
		cerr << e.what() << endl;
	} catch(const char *s) {
		cerr << s << endl;
	}
}
