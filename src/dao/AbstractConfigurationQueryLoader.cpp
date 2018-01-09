#include "dao/AbstractConfigurationQueryLoader.h"

using namespace std;
using namespace Poco::Util;
using namespace BeeeOn;

AbstractConfigurationQueryLoader::AbstractConfigurationQueryLoader():
	m_config(new LayeredConfiguration)
{
}

void AbstractConfigurationQueryLoader::addSourceFile(const string &path)
{
	m_config->add(loadConfig(path));
}

void AbstractConfigurationQueryLoader::setDatabase(const string &name)
{
	m_database = name;
}

void AbstractConfigurationQueryLoader::prepare()
{
	m_view = m_config->createView(m_database);
}

string AbstractConfigurationQueryLoader::find(const string &key) const
{
	return m_view->getString(key, "");
}
