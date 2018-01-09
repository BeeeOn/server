#include <Poco/Util/IniFileConfiguration.h>

#include "di/Injectable.h"
#include "dao/IniFileQueryLoader.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, IniFileQueryLoader)
BEEEON_OBJECT_CASTABLE(QueryLoader)
BEEEON_OBJECT_TEXT("file", &IniFileQueryLoader::addSourceFile)
BEEEON_OBJECT_TEXT("database", &IniFileQueryLoader::setDatabase)
BEEEON_OBJECT_HOOK("done", &IniFileQueryLoader::prepare)
BEEEON_OBJECT_END(BeeeOn, IniFileQueryLoader)

IniFileQueryLoader::IniFileQueryLoader():
	m_config(new LayeredConfiguration())
{
}

IniFileQueryLoader::~IniFileQueryLoader()
{
}

void IniFileQueryLoader::addSourceFile(const string &path)
{
	AutoPtr<IniFileConfiguration> config(new IniFileConfiguration(path));
	m_config->add(config);
}

void IniFileQueryLoader::setDatabase(const string &name)
{
	m_database = name;
}

void IniFileQueryLoader::prepare()
{
	m_view = m_config->createView(m_database);
}

string IniFileQueryLoader::find(const string &key) const
{
	return m_view->getString(key, "");
}
