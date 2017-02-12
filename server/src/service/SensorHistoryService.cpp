#include <Poco/Exception.h>

#include "dao/SensorHistoryDao.h"
#include "dao/DeviceDao.h"
#include "policy/SensorHistoryAccessPolicy.h"
#include "service/SensorHistoryService.h"
#include "service/ValueConsumer.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT(SensorHistoryService, BeeeOn::SensorHistoryService)

SensorHistoryService::SensorHistoryService():
	m_dao(&NullSensorHistoryDao::instance()),
	m_deviceDao(&NullDeviceDao::instance()),
	m_policy(&NullSensorHistoryAccessPolicy::instance())
{
	injector("sensorHistoryDao", &SensorHistoryService::setSensorHistoryDao);
	injector("deviceDao", &SensorHistoryService::setDeviceDao);
	injector("accessPolicy", &SensorHistoryService::setAccessPolicy);
}

void SensorHistoryService::setSensorHistoryDao(SensorHistoryDao *dao)
{
	m_dao = dao == NULL?
		&NullSensorHistoryDao::instance() : dao;
}

void SensorHistoryService::setDeviceDao(DeviceDao *dao)
{
	m_deviceDao = dao == NULL?
		&NullDeviceDao::instance() : dao;
}

void SensorHistoryService::setAccessPolicy(SensorHistoryAccessPolicy *policy)
{
	m_policy = policy == NULL?
		&NullSensorHistoryAccessPolicy::instance() : policy;
}

void SensorHistoryService::doFetchRange(
		const Relation<ModuleInfo, Device> &module,
		const TimeInterval &range,
		const Timespan &interval,
		const string &aggregator,
		ValueConsumer &consumer)
{
	m_policy->assureFetchRange(
		module, module.base(), module.target(), range);

	Device device(module.base());
	if (!m_deviceDao->fetch(device, device.gateway()))
		throw IllegalStateException("no such device " + device);

	ModuleInfo info(module.target());
	if (!device.type()->lookup(info))
		throw IllegalStateException("no such device module " + info);

	SensorHistoryDao::Aggregator agg;

	if (aggregator == "avg")
		agg = SensorHistoryDao::AGG_AVG;
	else if (aggregator == "min")
		agg = SensorHistoryDao::AGG_MIN;
	else if (aggregator == "max")
		agg = SensorHistoryDao::AGG_MAX;
	else
		throw InvalidArgumentException("invalid aggregator given");

	m_dao->fetchHuge(device, info, range, interval, agg, consumer);
}
