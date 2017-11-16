#include "model/AbstractModule.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

AbstractModule::AbstractModule()
{
}

AbstractModule::AbstractModule(const ID &id):
	Entity<SimpleID>(id)
{
}

void AbstractModule::setInfo(const ModuleInfo &info)
{
	m_info = info;
}

const ModuleInfo &AbstractModule::info() const
{
	return m_info;
}

void AbstractModule::setName(const string &name)
{
	m_name = name;
}

string AbstractModule::name() const
{
	return m_name;
}
