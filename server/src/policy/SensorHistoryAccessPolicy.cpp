#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "policy/SensorHistoryAccessPolicy.h"

using namespace Poco;
using namespace BeeeOn;

NullSensorHistoryAccessPolicy::NullSensorHistoryAccessPolicy()
{
}

void NullSensorHistoryAccessPolicy::assureFetchRange(
		const PolicyContext &context,
		const Device &device,
		const ModuleInfo &module,
		const TimeInterval &range)
{
	throw NotImplementedException(__func__);
}

SensorHistoryAccessPolicy &NullSensorHistoryAccessPolicy::instance()
{
	SingletonHolder<NullSensorHistoryAccessPolicy> singleton;
	return *singleton.get();
}
