#include "di/Injectable.h"
#include "service/GWSDeviceServiceImpl.h"
#include "util/MultiException.h"
#include "util/ZipIterator.h"

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
		status.setState(DeviceStatus::STATE_INACTIVE);
		status.setLastChanged({});

		return m_deviceDao->insert(device, gateway);
	}
}

void GWSDeviceServiceImpl::doFetchActiveWithPrefix(vector<Device> &devices,
		const Gateway &gateway,
		const DevicePrefix &prefix)
{
	m_deviceDao->fetchActiveWithPrefix(devices, gateway, prefix);
}

void GWSDeviceServiceImpl::verifyModules(
		const SharedPtr<DeviceInfo> deviceInfo,
		const list<ModuleType> &modules) const
{
	if (modules.size() > deviceInfo->modules().size()) {
		throw InvalidArgumentException("invalid count of modules "
			+ to_string(modules.size()) + " for device " + *deviceInfo);
	}

	MultiException ex;

	Zip<const set<ModuleInfo>, const list<ModuleType>> zip(
		deviceInfo->modules(), modules);

	auto i = 0;
	auto it = zip.begin();
	for (; it != zip.end(); ++it, ++i) {
		const auto &expect = (*it).first;
		const auto &given = (*it).second;

		if (expect.type()->name() != given.type().toString()) {
			ex.caught(InvalidArgumentException(
				"expected type " + expect.type()->name()
				+ " of module " + to_string(i) + " but " + given.type()
				+ " was given for device " + *deviceInfo
			));
		}
	}

	string missing;

	for (auto expectIt = it.firstIterator(); expectIt != zip.firstEnd(); ++expectIt, ++i) {
		if (!missing.empty())
			missing += ", ";

		missing += to_string(i) + " " + expectIt->type()->name();
	}

	if (!missing.empty()) {
		logger().warning("missing specification of " + missing
			 + " for device " + *deviceInfo);
	}

	if (!ex.empty())
		ex.rethrow();
}
