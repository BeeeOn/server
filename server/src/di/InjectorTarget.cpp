#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "Debug.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

ManifestSingleton *ManifestSingleton::singleton = NULL;

static void __attribute__((destructor,used)) ManifestSingleton_destroy()
{
	ManifestSingleton::destroy();
}

void ManifestSingleton::reportInfo(Logger &logger)
{
	logger.debug("managing " + to_string(manifest().size()) + " classes in " + manifest().className());
	Manifest<InjectorTarget>::Iterator it = manifest().begin();

	for (; it != manifest().end(); ++it) {
		string msg("registered class ");
		logger.debug(msg + it->name(), __FILE__, __LINE__);
	}
}
