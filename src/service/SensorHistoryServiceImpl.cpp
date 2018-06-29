#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "service/SensorHistoryServiceImpl.h"
#include "service/ValueConsumer.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, SensorHistoryServiceImpl)
BEEEON_OBJECT_CASTABLE(SensorHistoryService)
BEEEON_OBJECT_PROPERTY("sensorHistoryDao", &SensorHistoryServiceImpl::setSensorHistoryDao)
BEEEON_OBJECT_PROPERTY("deviceDao", &SensorHistoryServiceImpl::setDeviceDao)
BEEEON_OBJECT_PROPERTY("accessPolicy", &SensorHistoryServiceImpl::setAccessPolicy)
BEEEON_OBJECT_PROPERTY("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, SensorHistoryServiceImpl)

SensorHistoryServiceImpl::SensorHistoryServiceImpl()
{
}

void SensorHistoryServiceImpl::setSensorHistoryDao(SensorHistoryDao::Ptr dao)
{
	m_dao = dao;
}

void SensorHistoryServiceImpl::setDeviceDao(DeviceDao::Ptr dao)
{
	m_deviceDao = dao;
}

void SensorHistoryServiceImpl::setAccessPolicy(SensorHistoryAccessPolicy::Ptr policy)
{
	m_policy = policy;
}

void SensorHistoryServiceImpl::doFetchRange(
		const Relation<ModuleInfo, Device> &module,
		const TimeInterval &range,
		const Timespan &interval,
		const vector<string> &aggregators,
		ValueConsumer &consumer)
{
	m_policy->assure(SensorHistoryAccessPolicy::ACTION_USER_GET,
			module, module.base(), range, interval);

	Device device(module.base());
	if (!m_deviceDao->fetch(device, device.gateway()))
		throw IllegalStateException("no such device " + device);

	ModuleInfo info(module.target());
	if (!device.type()->lookup(info))
		throw IllegalStateException("no such device module " + info);

	vector<SensorHistoryDao::Aggregator> agg;

	for (const auto &one : aggregators) {
		if (one == "avg")
			agg.emplace_back(SensorHistoryDao::AGG_AVG);
		else if (one == "min")
			agg.emplace_back(SensorHistoryDao::AGG_MIN);
		else if (one == "max")
			agg.emplace_back(SensorHistoryDao::AGG_MAX);
		else if (one == "freq")
			agg.emplace_back(SensorHistoryDao::AGG_FREQ);
		else
			throw InvalidArgumentException("invalid aggregator given");
	}

	m_dao->fetchHuge(device, info, range, interval, agg, consumer);
}
