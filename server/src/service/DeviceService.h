#ifndef BEEEON_DEVICE_SERVICE_H
#define BEEEON_DEVICE_SERVICE_H

#include <vector>

#include "di/InjectorTarget.h"
#include "service/Relation.h"
#include "model/Device.h"
#include "model/Gateway.h"

namespace BeeeOn {

class DeviceDao;

/**
 * Service for devices management.
 */
class DeviceService : public AbstractInjectorTarget {
public:
	DeviceService();

	void setDeviceDao(DeviceDao *dao);

	bool fetch(Relation<Device, Gateway> &input);

private:
	DeviceDao *m_dao;
};

}

#endif
