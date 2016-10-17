#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "policy/PlaceAccessPolicy.h"

using namespace Poco;
using namespace BeeeOn;

void NullPlaceAccessPolicy::assureGet(
	const PolicyContext &context,
	const Place &place)
{
	throw NotImplementedException(__func__);
}

void NullPlaceAccessPolicy::assureUpdate(
	const PolicyContext &context,
	const Place &place)
{
	throw NotImplementedException(__func__);
}

void NullPlaceAccessPolicy::assureRemove(
	const PolicyContext &context,
	const Place &place)
{
	throw NotImplementedException(__func__);
}

PlaceAccessPolicy &NullPlaceAccessPolicy::instance()
{
	static SingletonHolder<NullPlaceAccessPolicy> singleton;
	return *singleton.get();
}
