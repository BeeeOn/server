#ifndef BEEEON_DEVICE_DAO_H
#define BEEEON_DEVICE_DAO_H

#include <vector>
#include <list>

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "model/Device.h"
#include "model/Gateway.h"

namespace BeeeOn {

class Gateway;

class DeviceDao {
public:
	virtual bool insert(Device &device, const Gateway &gateway) = 0;
	virtual bool update(Device &device, const Gateway &gateway) = 0;
	virtual bool fetch(Device &device, const Gateway &gateway) = 0;
	virtual void fetchMany(std::list<Device> &devices) = 0;
	virtual void fetchActiveBy(std::vector<Device> &devices,
			const Gateway &gateway) = 0;
	virtual void fetchInactiveBy(std::vector<Device> &devices,
			const Gateway &gateway) = 0;
};

class NullDeviceDao : public AbstractInjectorTarget,
	public NullDao<Device, DeviceDao> {
public:
	bool insert(Device &device, const Gateway &gateway)
	{
		return NullDao<Device, DeviceDao>::insert(device);
	}

	bool update(Device &device, const Gateway &gateway)
	{
		return NullDao<Device, DeviceDao>::update(device);
	}

	void fetchMany(std::list<Device> &devices) override;

	bool fetch(Device &device, const Gateway &gateway)
	{
		return NullDao<Device, DeviceDao>::fetch(device);
	}

	void fetchActiveBy(std::vector<Device> &devices,
			const Gateway &gateway);
	void fetchInactiveBy(std::vector<Device> &devices,
			const Gateway &gateway);

	static DeviceDao &instance();
};

}

#endif
