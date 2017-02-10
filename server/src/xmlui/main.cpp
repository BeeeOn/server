#include <Poco/Logger.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/Net/RejectCertificateHandler.h>

#include "di/DependencyInjector.h"
#include "loop/LoopRunner.h"
#include "util/Startup.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

class Startup : public ServerStartup {
public:
	Startup(): ServerStartup("beeeon", "ui-server", 0)
	{
	}

protected:
	int execute() override
	{
		if (logger().debug())
			ManifestSingleton::reportInfo(logger());

		if (m_serverPort != 0)
			config().setInt("xmlui.port", m_serverPort);

		DependencyInjector injector(config().createView("services"));

		SharedPtr<LoopRunner> runner = injector.create<LoopRunner>("xmlui");
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
