#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "service/DeviceService.h"
#include "model/Device.h"
#include "dao/DeviceDao.h"
#include "Debug.h"

BEEEON_OBJECT(DeviceService, BeeeOn::DeviceService)

using namespace BeeeOn;

DeviceService::DeviceService()
{
	injector<DeviceService, DeviceDao>("deviceDao",
			&DeviceService::setDeviceDao);
}

void DeviceService::setDeviceDao(DeviceDao *dao)
{
	m_dao = dao;
}

bool DeviceService::fetch(Device &device)
{
	TRACE_METHOD();
	return m_dao->fetch(device);
}
