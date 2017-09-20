#include <cmath>

#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "service/SensorServiceImpl.h"

BEEEON_OBJECT_BEGIN(BeeeOn, SensorServiceImpl)
BEEEON_OBJECT_CASTABLE(SensorService)
BEEEON_OBJECT_REF("deviceDao", &SensorServiceImpl::setDeviceDao)
BEEEON_OBJECT_REF("sensorHistoryDao", &SensorServiceImpl::setSensorHistoryDao)
BEEEON_OBJECT_REF("accessPolicy", &SensorServiceImpl::setAccessPolicy)
BEEEON_OBJECT_REF("transactionManager", &SensorServiceImpl::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, SensorServiceImpl)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

void SensorServiceImpl::setDeviceDao(DeviceDao::Ptr deviceDao)
{
	m_deviceDao = deviceDao;
}

void SensorServiceImpl::setSensorHistoryDao(SensorHistoryDao::Ptr sensorHistoryDao)
{
	m_sensorHistoryDao = sensorHistoryDao;
}

void SensorServiceImpl::setAccessPolicy(SensorAccessPolicy::Ptr policy)
{
	m_policy = policy;
}

bool SensorServiceImpl::doFetch(Relation<Sensor, Device> &data)
{
	Device device(data.base());
	Sensor &sensor = data.target();

	m_policy->assure(SensorAccessPolicy::ACTION_USER_GET,
			data, sensor, device);

	if (!m_deviceDao->fetch(device, device.gateway()))
		throw NotFoundException("device " + device);
		
	SharedPtr<DeviceInfo> deviceInfo = device.type();
	ModuleInfo moduleInfo;
	moduleInfo.setId(sensor.id());

	if (!deviceInfo->lookup(moduleInfo))
		return false;

	double value;
	Timestamp at;

	if (m_sensorHistoryDao->fetch(device, moduleInfo, at, value)) {
		sensor.setValue(value);
		sensor.setAt(at);
	}
	else {
		sensor.setValue(NAN);
		sensor.setAt(Nullable<Timestamp>());
	}

	sensor.setInfo(moduleInfo);
	return true;
}

void SensorServiceImpl::doFetchMany(Relation<list<Sensor>, Device> &data)
{
	Device device(data.base());

	m_policy->assureMany(SensorAccessPolicy::ACTION_USER_GET, data, device);

	if (!m_deviceDao->fetch(device, device.gateway()))
		throw NotFoundException("device " + device);
		
	SharedPtr<DeviceInfo> deviceInfo = device.type();
	vector<ModuleInfo> modules;

	for (const auto info : *deviceInfo)
		modules.emplace_back(info);

	vector<Nullable<ValueAt>> values;
	m_sensorHistoryDao->fetchMany(device, modules, values);

	for (size_t i = 0; i < values.size(); ++i) {
		Sensor sensor(modules[i].id());

		m_policy->assure(SensorAccessPolicy::ACTION_USER_GET, data, sensor, device);

		sensor.setInfo(modules[i]);

		if (!values[i].isNull()) {
			sensor.setValue(values[i].value().value());
			sensor.setAt(values[i].value().at());
		}
		else {
			sensor.setValue(NAN);
			sensor.setAt(Nullable<Timestamp>());
		}

		data.target().emplace_back(sensor);
	}
}
