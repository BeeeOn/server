#include <Poco/Util/IniFileConfiguration.h>

#include "di/Injectable.h"
#include "dao/IniFileQueryLoader.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, IniFileQueryLoader)
BEEEON_OBJECT_CASTABLE(QueryLoader)
BEEEON_OBJECT_PROPERTY("file", &IniFileQueryLoader::addSourceFile)
BEEEON_OBJECT_PROPERTY("database", &IniFileQueryLoader::setDatabase)
BEEEON_OBJECT_HOOK("done", &IniFileQueryLoader::prepare)
BEEEON_OBJECT_END(BeeeOn, IniFileQueryLoader)

IniFileQueryLoader::IniFileQueryLoader()
{
}

AutoPtr<AbstractConfiguration> IniFileQueryLoader::loadConfig(const string &path)
{
	return new IniFileConfiguration(path);
}
