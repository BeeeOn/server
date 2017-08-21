#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "work/WorkFacade.h"

using namespace Poco;
using namespace BeeeOn;

void NullWorkFacade::schedule(Work &)
{
	throw NotImplementedException(__func__);
}

void NullWorkFacade::wakeup(Work &)
{
	throw NotImplementedException(__func__);
}

void NullWorkFacade::cancel(Work &)
{
	throw NotImplementedException(__func__);
}

bool NullWorkFacade::fetch(Work &)
{
	throw NotImplementedException(__func__);
}

bool NullWorkFacade::remove(const Work &)
{
	throw NotImplementedException(__func__);
}

WorkFacade &NullWorkFacade::instance()
{
	SingletonHolder<NullWorkFacade> singleton;
	return *singleton.get();
}
