#include "di/Injectable.h"
#include "service/GWSSensorHistoryServiceImpl.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, GWSSensorHistoryServiceImpl)
BEEEON_OBJECT_CASTABLE(GWSSensorHistoryService)
BEEEON_OBJECT_REF("deviceDao", &GWSSensorHistoryServiceImpl::setDeviceDao)
BEEEON_OBJECT_REF("sensorHistoryDao", &GWSSensorHistoryServiceImpl::setSensorHistoryDao)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, GWSSensorHistoryServiceImpl)

void GWSSensorHistoryServiceImpl::setDeviceDao(DeviceDao::Ptr dao)
{
	m_deviceDao = dao;
}

void GWSSensorHistoryServiceImpl::setSensorHistoryDao(SensorHistoryDao::Ptr dao)
{
	m_sensorHistoryDao = dao;
}

void GWSSensorHistoryServiceImpl::doInsertMany(Device &device,
		const Poco::Timestamp &at,
		std::vector<ModuleValue> &values)
{
	if (!m_deviceDao->fetch(device, device.gateway()))
		throw NotFoundException("no such device " + device);

	device.setLastSeen(DateTime());
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
	}

	m_sensorHistoryDao->insertMany(device, at, values);
}

bool GWSSensorHistoryServiceImpl::doFetchLast(Device &device,
		ModuleInfo &module,
		Poco::Timestamp &at,
		double &value)
{
	if (!m_deviceDao->fetch(device, device.gateway()))
		throw NotFoundException("no such device " + device);

	if (!device.type()->lookup(module))
		throw NotFoundException("no such device module " + module);

	return m_sensorHistoryDao->fetch(device, module, at, value);
}
