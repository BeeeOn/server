#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "service/ControlServiceImpl.h"

BEEEON_OBJECT_BEGIN(BeeeOn, ControlServiceImpl)
BEEEON_OBJECT_CASTABLE(ControlService)
BEEEON_OBJECT_REF("controlDao", &ControlServiceImpl::setControlDao)
BEEEON_OBJECT_REF("deviceDao", &ControlServiceImpl::setDeviceDao)
BEEEON_OBJECT_REF("accessPolicy", &ControlServiceImpl::setAccessPolicy)
BEEEON_OBJECT_REF("transactionManager", &ControlServiceImpl::setTransactionManager)
BEEEON_OBJECT_HOOK("done", &ControlServiceImpl::fixupControls)
BEEEON_OBJECT_END(BeeeOn, ControlServiceImpl)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

void ControlServiceImpl::setDeviceDao(DeviceDao::Ptr dao)
{
	m_deviceDao = dao;
}

void ControlServiceImpl::setControlDao(ControlDao::Ptr dao)
{
	m_controlDao = dao;
}

void ControlServiceImpl::setAccessPolicy(ControlAccessPolicy::Ptr policy)
{
	m_accessPolicy = policy;
}

bool ControlServiceImpl::doFetch(Relation<Control, Device> &data)
{
	Control &control = data.target();
	Device device = data.base();

	m_accessPolicy->assure(
		ControlAccessPolicy::ACTION_USER_GET,
		data, control, device);

	if (!m_deviceDao->fetch(device, device.gateway()))
		throw NotFoundException("device " + device);

	return m_controlDao->fetch(data.target(), device);
}

void ControlServiceImpl::doFetchMany(Relation<std::list<Control>, Device> &data)
{
	Device device = data.base();

	m_accessPolicy->assureMany(ControlAccessPolicy::ACTION_USER_GET, data, device);

	if (!m_deviceDao->fetch(device, device.gateway()))
		throw NotFoundException("device " + device);

	std::list<Control> result;
	m_controlDao->fetchBy(result, device);

	for (const auto &control : result) {
		try {
			m_accessPolicy->assure(ControlAccessPolicy::ACTION_USER_GET,
					data, control, device);
		} catch (const Exception &e) {
			if (logger().debug()) // only while debugging
				logger().log(e, __FILE__, __LINE__);

			continue;
		}

		data.target().emplace_back(control);
	}
}

void ControlServiceImpl::fixupControls()
{
	size_t count = BEEEON_TRANSACTION_RETURN(size_t,
				m_controlDao->cancelUnfinished());

	if (count > 0) {
		logger().warning("cancelled "
				+ to_string(count)
				+ " control requests",
			__FILE__, __LINE__);
	}
}
