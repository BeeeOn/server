#include "di/Injectable.h"
#include "service/GWSDeviceServiceImpl.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, GWSDeviceServiceImpl)
BEEEON_OBJECT_CASTABLE(GWSDeviceService)
BEEEON_OBJECT_REF("deviceDao", &GWSDeviceServiceImpl::setDeviceDao)
BEEEON_OBJECT_REF("deviceInfoProvider", &GWSDeviceServiceImpl::setDeviceInfoProvider)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, GWSDeviceServiceImpl)

void GWSDeviceServiceImpl::setDeviceDao(DeviceDao::Ptr dao)
{
	m_deviceDao = dao;
}

void GWSDeviceServiceImpl::setDeviceInfoProvider(DeviceInfoProvider::Ptr provider)
{
	m_deviceInfoProvider = provider;
}

bool GWSDeviceServiceImpl::doRegisterDevice(Device &device,
		const string &name,
		const string &vendor,
		const Gateway &gateway)
{
	if (m_deviceDao->fetch(device, gateway)) {
		device.status().setLastSeen(Timestamp());

		return m_deviceDao->update(device, gateway);
	}
	else {
		device.setName(name);

		auto type = m_deviceInfoProvider->findByNameAndVendor(name, vendor);
		if (type.isNull()) {
			throw NotFoundException("no such device type for "
				"'" + vendor + "' '" + name + "' specification");
		}

		device.setType(type);

		DeviceStatus &status = device.status();

		status.setFirstSeen(Timestamp());
		status.setLastSeen(Timestamp());

		status.setActiveSince(Nullable<Timestamp>());

		return m_deviceDao->insert(device, gateway);
	}
}

void GWSDeviceServiceImpl::doFetchActiveWithPrefix(vector<Device> &devices,
		const Gateway &gateway,
		const DevicePrefix &prefix)
{
	m_deviceDao->fetchActiveWithPrefix(devices, gateway, prefix);
}
