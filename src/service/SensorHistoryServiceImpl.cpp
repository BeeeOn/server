#include <Poco/Exception.h>
#include <Poco/String.h>

#include "di/Injectable.h"
#include "service/SensorHistoryServiceImpl.h"
#include "service/ValueConsumer.h"
#include "util/MultiException.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, SensorHistoryServiceImpl)
BEEEON_OBJECT_CASTABLE(SensorHistoryService)
BEEEON_OBJECT_CASTABLE(GWSSensorHistoryService)
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
	m_sensorHistoryDao = dao;
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

	m_sensorHistoryDao->fetchHuge(device, info, range, interval, agg, consumer);
}

void SensorHistoryServiceImpl::doInsertMany(Device &device,
		const Poco::Timestamp &at,
		std::vector<ModuleValue> &values)
{
	if (!m_deviceDao->fetch(device, device.gateway()))
		throw NotFoundException("no such device " + device);

	device.status().setLastSeen(Timestamp());
	if (!m_deviceDao->update(device, device.gateway()))
		throw IllegalStateException("updating device " + device + " failed");

	for (auto &value : values) {
		ModuleInfo module;
		module.setId(value.module());

		if (!device.type()->lookup(module)) {
			throw InvalidArgumentException(
				"device type " + device.type()->vendor()
				+ " " + device.type()->name()
				+ " has not module " + value.module().toString());
		}

		if (!value.isValid() || module.fromUnknown().empty())
			continue;

		value.setValue(m_unknownEvaluator.fromRaw(module, value.value()));
	}

	try {
		m_sensorHistoryDao->insertMany(device, at, values);
	}
	catch (const MultiException &e) {
		logger().error("failed to insert "
				+ to_string(e.count())
				+ " of "
				+ to_string(values.size())
				+ " values for "
				+ device,
				__FILE__, __LINE__);

		logger().log(e, __FILE__, __LINE__);
	}
	catch (const Exception &e) {
		logger().error("failed to insert value for " + device,
				__FILE__, __LINE__);
		logger().log(e, __FILE__, __LINE__);
	}
}

bool SensorHistoryServiceImpl::doFetchLast(Device &device,
		ModuleInfo &module,
		Poco::Timestamp &at,
		double &raw)
{
	if (!m_deviceDao->fetch(device, device.gateway()))
		throw NotFoundException("no such device " + device);

	if (!device.type()->lookup(module))
		throw NotFoundException("no such device module " + module);

	double value;
	if (!m_sensorHistoryDao->fetch(device, module, at, value))
		return false;

	if (!std::isnan(value) && !module.toUnknown().empty())
		raw = m_unknownEvaluator.toRaw(module, value);
	else
		raw = value;

	return true;
}
