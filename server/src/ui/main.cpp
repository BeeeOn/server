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

#define LOCAL_LOGGING_FILE  "logging.ini"
#define SYSTEM_LOGGING_FILE "/etc/beeeon/ui-server/logging.ini"

#define LOCAL_SERVICES_FILE  "services.xml"
#define SYSTEM_SERVICES_FILE "/etc/beeeon/ui-server/services.xml"

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
		if (name == "help")
			m_printHelp = true;
		if (name == "port")
			m_serverPort = stoi(value);

		Application::handleOption(name, value);
	}

	void findAndLoadLogging()
	{
		File user(m_userLogging);
		File local(LOCAL_LOGGING_FILE);
		File system(SYSTEM_LOGGING_FILE);

		if (!m_userLogging.empty() && user.exists())
			loadConfiguration(user.path());
		if (local.exists())
			loadConfiguration(local.path());
		else if (system.exists())
			loadConfiguration(system.path());
	}

	void findAndLoadServices()
	{
		File user(m_userServices);
		File local(LOCAL_SERVICES_FILE);
		File system(SYSTEM_SERVICES_FILE);

		if (!m_userServices.empty() && user.exists()) {
			logger().notice(
				"loading configuration from " + user.path(),
				__FILE__, __LINE__);
			loadConfiguration(user.path());
		}
		else if (local.exists()) {
			logger().notice(
				"loading configuration from " + local.path(),
				__FILE__, __LINE__);
			loadConfiguration(local.path());
		}
		else if (system.exists()) {
			logger().notice(
				"loading configuration from " + system.path(),
				__FILE__, __LINE__);
			loadConfiguration(system.path());
		}
	}

	void initialize(Application &app)
	{
		Logger::root().setLevel(Logger::parseLevel("trace"));
		findAndLoadLogging();
		Application::initialize(app);
		findAndLoadServices();
	}

	void defineOptions(OptionSet &options)
	{
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
