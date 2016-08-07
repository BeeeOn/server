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
	virtual Device::Ptr get(DeviceID id) = 0;
};

class NullDeviceDao : public AbstractInjectorTarget,
	public NullDao<Device, DeviceDao> {
};

class MockDeviceDao : public AbstractInjectorTarget,
	public MockDao<Device, DeviceDao> {
};

}

#endif
