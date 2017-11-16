#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>
#include <Poco/Timespan.h>

#include "work/WorkExecutor.h"
#include "work/WorkSuspendThrowable.h"

using namespace Poco;
using namespace BeeeOn;

WorkExecutor::~WorkExecutor()
{
}

void WorkExecutor::suspend(Work::Ptr)
{
	throw WorkSuspendForEventThrowable();
}

void WorkExecutor::suspend(
		Work::Ptr,
		const Timespan &duration)
{
	throw WorkSuspendThrowable(duration);
}

NullWorkExecutor::NullWorkExecutor()
{
}

bool NullWorkExecutor::accepts(const Work::Ptr) const
{
	throw NotImplementedException(__func__);
}

void NullWorkExecutor::execute(Work::Ptr)
{
	throw NotImplementedException(__func__);
}

WorkExecutor &NullWorkExecutor::instance()
{
	static SingletonHolder<NullWorkExecutor> singleton;
	return *singleton.get();
}
