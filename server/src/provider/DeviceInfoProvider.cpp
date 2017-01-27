#include "provider/DeviceInfoProvider.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

BEEEON_OBJECT(DeviceInfoProvider, BeeeOn::DeviceInfoProvider)

DeviceInfoProvider::DeviceInfoProvider():
	m_typeProvider(&NullInfoProvider<TypeInfo>::instance())
{
	textInjector("devicesFile", (TextSetter) &DeviceInfoProvider::setDevicesFile);

	injector<DeviceInfoProvider, InfoProvider<TypeInfo>>(
		"typeInfoProvider",
		&DeviceInfoProvider::setTypeInfoProvider
	);
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

void DeviceInfoProvider::injectionDone()
{
	parseFile(m_devicesFile, "device");

	InfoProvider<DeviceInfo>::InfoSet set;

	for (auto &device : infoSet())
		set.insert(new DeviceInfo(resolveTypes(*device)));

	infoSet() = set;
}
