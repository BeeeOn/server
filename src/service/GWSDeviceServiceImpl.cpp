#include "di/Injectable.h"
#include "service/GWSDeviceServiceImpl.h"
#include "util/MultiException.h"
#include "util/ZipIterator.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, GWSDeviceServiceImpl)
BEEEON_OBJECT_CASTABLE(GWSDeviceService)
BEEEON_OBJECT_PROPERTY("deviceDao", &GWSDeviceServiceImpl::setDeviceDao)
BEEEON_OBJECT_PROPERTY("deviceInfoProvider", &GWSDeviceServiceImpl::setDeviceInfoProvider)
BEEEON_OBJECT_PROPERTY("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, GWSDeviceServiceImpl)

void GWSDeviceServiceImpl::setDeviceDao(DeviceDao::Ptr dao)
{
	m_deviceDao = dao;
}

void GWSDeviceServiceImpl::setDeviceInfoProvider(DeviceInfoProvider::Ptr provider)
{
	m_deviceInfoProvider = provider;
}

SharedPtr<DeviceInfo> GWSDeviceServiceImpl::verifyDescription(
		const DeviceDescription &description) const
{
	auto type = m_deviceInfoProvider->findByNameAndVendor(
		description.productName(),
		description.vendor());
	if (type.isNull()) {
		throw NotFoundException("no such device type for "
			"'" + description.toString() + "' specification");
	}

	verifyModules(type, description.dataTypes());
	return type;
}

bool GWSDeviceServiceImpl::doRegisterDevice(Device &device,
		const DeviceDescription &description,
		const Gateway &gateway)
{
	if (m_deviceDao->fetch(device, gateway)) {
		auto type = verifyDescription(description);

		if (type != device.type()) {
			throw IllegalStateException(
				"description " + description.toString()
				+ " has non-matching device type for device " + device);
		}

		device.status().setLastSeen(Timestamp());

		return m_deviceDao->update(device, gateway);
	}
	else {
		device.setName(description.productName());
		device.setType(verifyDescription(description));

		DeviceStatus &status = device.status();

		status.setFirstSeen(Timestamp());
		status.setLastSeen(Timestamp());
		status.setState(DeviceStatus::STATE_INACTIVE);
		status.setLastChanged({});

		return m_deviceDao->insert(device, gateway);
	}
}

void GWSDeviceServiceImpl::doRegisterDeviceGroup(
		const vector<DeviceDescription> &descriptions,
		const Gateway &gateway)
{
	for (auto &des : descriptions) {
		Device device(des.id());
		device.setRefresh(des.refreshTime());

		if (!doRegisterDevice(device, des, gateway)) {
			throw IllegalStateException("registration of device "
				"'" + des.toString() + "' failed");
		}
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
