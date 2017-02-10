#include <Poco/Logger.h>

#include "util/Startup.h"
#include "di/DependencyInjector.h"
#include "UIServerModule.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

class Startup : public ServerStartup {
public:
	Startup(): ServerStartup("BeeeOn", "ui-server", 0)
	{
	}

protected:
	int execute() override
	{
		if (logger().debug())
			ManifestSingleton::reportInfo(logger());

		if (m_serverPort > 0)
			config().setInt("ui.port", m_serverPort);

		DependencyInjector injector(config().createView("services"));
		UIServerModule *module = injector
					.create<UIServerModule>("ui");

		module->createServer();
		module->server().start();

		notifyStarted();

		waitForTerminationRequest();
		module->server().stop();
		return EXIT_OK;
	}
};

int main(int argc, char **argv)
{
	Startup startup;
	return generic_main(argc, argv, startup);
}
