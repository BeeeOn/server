#include <Poco/SharedPtr.h>
#include <Poco/Logger.h>

#include "util/Startup.h"
#include "di/DependencyInjector.h"
#include "loop/LoopRunner.h"

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
		SharedPtr<LoopRunner> runner = injector.create<LoopRunner>("ui");

		runner->start();

		notifyStarted();

		waitForTerminationRequest();
		runner->stop();
		return EXIT_OK;
	}
};

int main(int argc, char **argv)
{
	Startup startup;
	return generic_main(argc, argv, startup);
}
