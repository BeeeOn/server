#ifndef BEEEON_DEVICE_SERVICE_H
#define BEEEON_DEVICE_SERVICE_H

#include "di/InjectorTarget.h"

namespace BeeeOn {

class DeviceDao;
class Device;

/**
 * Service for devices management.
 */
class DeviceService : public AbstractInjectorTarget {
public:
	DeviceService();

	void setDeviceDao(DeviceDao *dao);

	bool fetch(Device &device);

private:
	DeviceDao *m_dao;
};

}

#endif
