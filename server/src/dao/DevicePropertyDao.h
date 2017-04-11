#ifndef BEEEON_DEVICE_PROPERTY_DAO_H
#define BEEEON_DEVICE_PROPERTY_DAO_H

#include <list>

#include "model/DeviceProperty.h"

namespace BeeeOn {

class Device;

class DevicePropertyDao {
public:
	virtual bool insert(DeviceProperty &property, const Device &device) = 0;
	virtual bool update(DeviceProperty &property, const Device &device) = 0;
	virtual bool remove(const DeviceProperty &property, const Device &device) = 0;
	virtual bool fetch(DeviceProperty &property, const Device &device) = 0;
	virtual void fetchByDevice(std::list<DeviceProperty> &properties, const Device &device) = 0;
};

class NullDevicePropertyDao : public DevicePropertyDao {
public:
	bool insert(DeviceProperty &property, const Device &device) override;
	bool update(DeviceProperty &property, const Device &device) override;
	bool remove(const DeviceProperty &property, const Device &device) override;
	bool fetch(DeviceProperty &property, const Device &device) override;
	void fetchByDevice(std::list<DeviceProperty> &properties, const Device &device) override;

	static DevicePropertyDao &instance();
};

}

#endif
