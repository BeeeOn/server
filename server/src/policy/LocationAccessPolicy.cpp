#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "policy/LocationAccessPolicy.h"

using namespace Poco;
using namespace BeeeOn;

void NullLocationAccessPolicy::assureGet(
	const PolicyContext &context,
	const Location &location)
{
	throw NotImplementedException(__func__);
}

void NullLocationAccessPolicy::assureCreateLocation(
	const PolicyContext &context,
	const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullLocationAccessPolicy::assureUpdate(
	const PolicyContext &context,
	const Location &location)
{
	throw NotImplementedException(__func__);
}

void NullLocationAccessPolicy::assureRemove(
	const PolicyContext &context,
	const Location &location)
{
	throw NotImplementedException(__func__);
}

LocationAccessPolicy &NullLocationAccessPolicy::instance()
{
	static SingletonHolder<NullLocationAccessPolicy> singleton;
	return *singleton.get();
}
