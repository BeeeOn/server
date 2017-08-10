#include <Poco/Exception.h>

#include "dao/SensorHistoryDao.h"
#include "di/Injectable.h"
#include "service/SensorHistoryServiceImpl.h"
#include "service/ValueConsumer.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, SensorHistoryServiceImpl)
BEEEON_OBJECT_CASTABLE(SensorHistoryService)
BEEEON_OBJECT_REF("sensorHistoryDao", &SensorHistoryServiceImpl::setSensorHistoryDao)
BEEEON_OBJECT_REF("deviceDao", &SensorHistoryServiceImpl::setDeviceDao)
BEEEON_OBJECT_REF("accessPolicy", &SensorHistoryServiceImpl::setAccessPolicy)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, SensorHistoryServiceImpl)

SensorHistoryServiceImpl::SensorHistoryServiceImpl():
	m_dao(&NullSensorHistoryDao::instance())
{
}

void SensorHistoryServiceImpl::setSensorHistoryDao(SensorHistoryDao *dao)
{
	m_dao = dao == NULL?
		&NullSensorHistoryDao::instance() : dao;
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
		const string &aggregator,
		ValueConsumer &consumer)
{
	m_policy->assure(SensorHistoryAccessPolicy::ACTION_USER_GET,
			module, module.base());

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
