#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "service/DeviceService.h"
#include "model/Device.h"
#include "model/Gateway.h"
#include "dao/DeviceDao.h"
#include "Debug.h"

BEEEON_OBJECT(DeviceService, BeeeOn::DeviceService)

using namespace BeeeOn;

DeviceService::DeviceService():
	m_dao(&NullDeviceDao::instance())
{
	injector<DeviceService, DeviceDao>("deviceDao",
			&DeviceService::setDeviceDao);
}

void DeviceService::setDeviceDao(DeviceDao *dao)
{
	m_dao = dao? dao : &NullDeviceDao::instance();
}

bool DeviceService::fetch(Relation<Device, Gateway> &input)
{
	TRACE_METHOD();

	return m_dao->fetch(input.target(), input.base());
}
