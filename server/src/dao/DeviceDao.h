#ifndef BEEEON_DEVICE_DAO_H
#define BEEEON_DEVICE_DAO_H

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "model/Place.h"
#include "model/Device.h"

namespace BeeeOn {

class DeviceDao {
public:
	virtual bool fetch(Device &device) = 0;
};

class NullDeviceDao : public AbstractInjectorTarget,
	public NullDao<Device, DeviceDao> {
public:
	static DeviceDao &instance();
};

class MockDeviceDao : public AbstractInjectorTarget,
	public MockDao<Device, DeviceDao> {
protected:
	DeviceID nextID()
	{
		return DeviceID::random();
	}

private:
	DeviceID m_id;
};

}

#endif
