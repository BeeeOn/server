#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "work/WorkScheduler.h"

using namespace Poco;
using namespace BeeeOn;

WorkScheduler::~WorkScheduler()
{
}

void NullWorkScheduler::schedule(Work::Ptr)
{
	throw NotImplementedException(__func__);
}

void NullWorkScheduler::wakeup(Work::Ptr)
{
	throw NotImplementedException(__func__);
}

void NullWorkScheduler::cancel(Work::Ptr)
{
	throw NotImplementedException(__func__);
}

void NullWorkScheduler::notify()
{
	throw NotImplementedException(__func__);
}

WorkScheduler &NullWorkScheduler::instance()
{
	SingletonHolder<NullWorkScheduler> singleton;
	return *singleton.get();
}
