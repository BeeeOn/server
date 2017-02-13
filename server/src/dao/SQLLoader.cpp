#include <Poco/Util/IniFileConfiguration.h>

#include "dao/SQLLoader.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace BeeeOn;

BEEEON_OBJECT(BeeeOn, SQLLoader)

SQLLoader::SQLLoader():
	m_config(new LayeredConfiguration())
{
	textInjector("file", &SQLLoader::addSourceFile);
	textInjector("database", &SQLLoader::setDatabase);
	hook("done", &SQLLoader::prepare);
}

SQLLoader::~SQLLoader()
{
}

void SQLLoader::addSourceFile(const string &path)
{
	AutoPtr<IniFileConfiguration> config(new IniFileConfiguration(path));
	m_config->add(config);
}

void SQLLoader::setDatabase(const string &name)
{
	m_database = name;
}

void SQLLoader::prepare()
{
	m_view = m_config->createView(m_database);
}

string SQLLoader::find(const string &key) const
{
	return m_view->getString(key, "");
}
