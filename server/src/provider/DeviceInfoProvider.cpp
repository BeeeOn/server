#include "di/Injectable.h"
#include "provider/DeviceInfoProvider.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, DeviceInfoProvider)
BEEEON_OBJECT_CASTABLE(InfoProvider<DeviceInfo>)
BEEEON_OBJECT_TEXT("devicesFile", &DeviceInfoProvider::setDevicesFile)
BEEEON_OBJECT_REF("typeInfoProvider", &DeviceInfoProvider::setTypeInfoProvider)
BEEEON_OBJECT_HOOK("done", &DeviceInfoProvider::loadInfo)
BEEEON_OBJECT_END(BeeeOn, DeviceInfoProvider)

DeviceInfoProvider::DeviceInfoProvider():
	m_typeProvider(&NullInfoProvider<TypeInfo>::instance())
{
}

void DeviceInfoProvider::setDevicesFile(const std::string &devicesFile)
{
	m_devicesFile = devicesFile;
}

void DeviceInfoProvider::setTypeInfoProvider(InfoProvider<TypeInfo> *provider)
{
	m_typeProvider = provider == NULL?
		&NullInfoProvider<TypeInfo>::instance() : provider;
}

DeviceInfo DeviceInfoProvider::resolveTypes(const DeviceInfo &device)
{
	DeviceInfo result(device);
	result.clear();

	for (auto &module : device) {
		const TypeInfoID &id = module.type()->id();
		const SharedPtr<TypeInfo> info = m_typeProvider->findById(id);

		if (info.isNull()) {
			logger().warning("could not resolve type " + id.toString()
				+ " for device " + device,
				__FILE__, __LINE__);

			continue;
		}

		ModuleInfo copy(module);
		copy.setType(info);
		result.add(copy);
	}

	return result;
}

void DeviceInfoProvider::loadInfo()
{
	parseFile(m_devicesFile, "device");

	InfoProvider<DeviceInfo>::InfoSet set;

	for (auto &device : infoSet())
		set.insert(new DeviceInfo(resolveTypes(*device)));

	infoSet() = set;
}

const SharedPtr<DeviceInfo> DeviceInfoProvider::findByNameAndVendor(
		const std::string &name, const std::string &vendor) const
{
	for (auto info : infoSet()) {
		if (info->match(name, vendor))
			return info;
	}

	return NULL;
}
