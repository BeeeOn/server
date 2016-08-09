#ifndef BEEEON_DEVICE_SERVICE_H
#define BEEEON_DEVICE_SERVICE_H

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "dao/DeviceDao.h"
#include "di/InjectorTarget.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * Service for devices management.
 */
class DeviceService : public AbstractInjectorTarget {
public:
	DeviceService()
	{
		injector<DeviceService, DeviceDao>("deviceDao",
				&DeviceService::setDeviceDao);
	}

	void setDeviceDao(DeviceDao *dao)
	{
		m_dao = dao;
	}

	bool fetch(Device &device)
	{
		TRACE_METHOD();
		return m_dao->fetch(device);
	}

private:
	DeviceDao *m_dao;
};

}

#endif