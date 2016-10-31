#ifndef BEEEON_DEVICE_DAO_H
#define BEEEON_DEVICE_DAO_H

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "model/Place.h"
#include "model/Device.h"
#include "model/Gateway.h"

namespace BeeeOn {

class DeviceDao {
public:
	virtual bool fetch(Device &device, const Gateway &gateway) = 0;
};

class NullDeviceDao : public AbstractInjectorTarget,
	public NullDao<Device, DeviceDao> {
public:
	bool fetch(Device &device, const Gateway &gateway)
	{
		return NullDao<Device, DeviceDao>::fetch(device);
	}

	static DeviceDao &instance();
};

class MockDeviceDao : public AbstractInjectorTarget,
	public MockRelationDao<Device, Gateway, DeviceDao> {
protected:
	DeviceID nextID()
	{
		return DeviceID::random();
	}

	void setBase(Device &device, const Gateway &gateway)
	{
		device.setGateway(gateway);
	}

private:
	DeviceID m_id;
};

}

#endif
