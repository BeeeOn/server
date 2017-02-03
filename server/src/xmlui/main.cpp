#include <Poco/Logger.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/Net/RejectCertificateHandler.h>

#include "di/DependencyInjector.h"
#include "server/SocketServer.h"
#include "server/XmlRequestHandler.h"
#include "ssl/SSLServer.h"
#include "util/Startup.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

#define DEFAULT_PORT 8001

class Startup : public ServerStartup {
public:
	Startup(): ServerStartup("beeeon", "ui-server", DEFAULT_PORT)
	{
	}

protected:
	SocketServer *createSocketServer(
			DependencyInjector &injector,
			XmlRequestHandlerFactory::Ptr factory)
	{
		SSLServer *sslConfig = NULL;

		if (config().getBool("xmlui.ssl.enable", false))
			sslConfig = injector.create<SSLServer>("xmluiSSLServer");

		return SocketServer::create(factory, sslConfig, m_serverPort);
	}

	int execute() override
	{
		if (logger().debug())
			ManifestSingleton::reportInfo(logger());

		DependencyInjector injector(config().createView("services"));
		XmlRequestHandlerFactory::Ptr factory = injector
			.create<XmlRequestHandlerFactory>("xmlui");

		SocketServer *server = createSocketServer(injector, factory);
		server->start();

		notifyStarted();

		waitForTerminationRequest();
		server->stop();
		delete server;

		return EXIT_OK;
	}
};

int main(int argc, char **argv)
{
	Startup startup;
	return generic_main(argc, argv, startup);
}
