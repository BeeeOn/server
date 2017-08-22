#ifndef BEEEON_DEVICE_DAO_H
#define BEEEON_DEVICE_DAO_H

#include <vector>
#include <list>

#include <Poco/SharedPtr.h>

#include "model/Device.h"
#include "model/Gateway.h"

namespace BeeeOn {

class Gateway;

class DeviceDao {
public:
	typedef Poco::SharedPtr<DeviceDao> Ptr;

	virtual ~DeviceDao();

	virtual bool insert(Device &device, const Gateway &gateway) = 0;
	virtual bool update(Device &device, const Gateway &gateway) = 0;
	virtual bool fetch(Device &device, const Gateway &gateway) = 0;
	virtual void fetchMany(std::list<Device> &devices) = 0;
	virtual void fetchActiveBy(std::vector<Device> &devices,
			const Gateway &gateway) = 0;
	virtual void fetchInactiveBy(std::vector<Device> &devices,
			const Gateway &gateway) = 0;
	virtual void fetchActiveWithPrefix(std::vector<Device> &devices,
			const Gateway &gateway,
			const DevicePrefix &prefix) = 0;
};

}

#endif
