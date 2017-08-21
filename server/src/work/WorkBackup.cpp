#include <Poco/SingletonHolder.h>

#include "work/WorkBackup.h"

using namespace Poco;
using namespace BeeeOn;

WorkBackup::~WorkBackup()
{
}

void EmptyWorkBackup::store(Work::Ptr, bool)
{
}

void EmptyWorkBackup::loadScheduled(std::vector<Work::Ptr> &)
{
}

WorkBackup &EmptyWorkBackup::instance()
{
	static SingletonHolder<EmptyWorkBackup> singleton;
	return *singleton.get();
}
