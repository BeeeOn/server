#pragma once

#include <list>

#include <Poco/SharedPtr.h>

#include "model/DeviceProperty.h"

namespace BeeeOn {

class Device;

class DevicePropertyDao {
public:
	typedef Poco::SharedPtr<DevicePropertyDao> Ptr;

	virtual ~DevicePropertyDao();

	virtual bool insert(DeviceProperty &property, const Device &device) = 0;
	virtual bool update(DeviceProperty &property, const Device &device) = 0;
	virtual bool remove(const DeviceProperty &property, const Device &device) = 0;
	virtual bool fetch(DeviceProperty &property, const Device &device) = 0;
	virtual void fetchByDevice(std::list<DeviceProperty> &properties, const Device &device) = 0;
};

}
