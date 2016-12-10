#include <Poco/Logger.h>
#include <Poco/SignalHandler.h>

#include "util/Startup.h"
#include "di/DependencyInjector.h"
#include "UIServerModule.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

#define DEFAULT_PORT 8000

class Startup : public ServerStartup {
public:
	Startup(): ServerStartup("BeeeOn", "ui-server", DEFAULT_PORT)
	{
	}

protected:
	int execute() override
	{
		if (logger().debug())
			ManifestSingleton::reportInfo(logger());

		DependencyInjector injector(config().createView("services"));
		UIServerModule *module = injector
					.create<UIServerModule>("ui");

		module->createServer(m_serverPort);
		module->server().start();

		waitForTerminationRequest();
		module->server().stop();
		return EXIT_OK;
	}
};

int main(int argc, char **argv)
{
	try {
		poco_throw_on_signal;

		Startup server;
		server.setUnixOptions(true);

		return server.run(argc, argv);
	} catch(Exception &e) {
		cerr << e.displayText() << endl;
	} catch(exception &e) {
		cerr << e.what() << endl;
	} catch(const char *s) {
		cerr << s << endl;
	}
}
