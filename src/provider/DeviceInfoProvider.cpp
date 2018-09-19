#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "model/ModuleType.h"
#include "provider/DeviceInfoProvider.h"
#include "util/DevicesSAXHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, DeviceInfoProvider)
BEEEON_OBJECT_CASTABLE(InfoProvider<DeviceInfo>)
BEEEON_OBJECT_PROPERTY("devicesFile", &DeviceInfoProvider::setDevicesFile)
BEEEON_OBJECT_PROPERTY("typeInfoProvider", &DeviceInfoProvider::setTypeInfoProvider)
BEEEON_OBJECT_PROPERTY("subtypeInfoProvider", &DeviceInfoProvider::setSubtypeInfoProvider)
BEEEON_OBJECT_HOOK("done", &DeviceInfoProvider::loadInfo)
BEEEON_OBJECT_END(BeeeOn, DeviceInfoProvider)

DeviceInfoProvider::DeviceInfoProvider():
	m_typeProvider(&NullInfoProvider<TypeInfo>::instance()),
	m_subtypeProvider(&NullInfoProvider<SubtypeInfo>::instance())
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

void DeviceInfoProvider::setSubtypeInfoProvider(InfoProvider<SubtypeInfo> *provider)
{
	m_subtypeProvider = provider == NULL?
		&NullInfoProvider<SubtypeInfo>::instance() : provider;
}

DeviceInfo DeviceInfoProvider::resolveTypes(const DeviceInfo &device)
{
	static const TypeInfoID TYPE_ENUM_ID = ModuleType::Type::TYPE_ENUM;

	DeviceInfo result(device);
	result.clear();

	for (auto &module : device) {
		const TypeInfoID &id = module.type()->id();
		const SharedPtr<TypeInfo> info = m_typeProvider->findById(id);

		if (info.isNull()) {
			throw IllegalStateException(
				"could not resolve type " + id.toString()
				+ " for device " + device);
		}

		ModuleInfo copy(module);
		copy.setType(info);

		if (id == TYPE_ENUM_ID) {
			if (module.subtype().isNull()) {
				throw IllegalStateException(
					"missing subtype for enum for device " + device);
			}

			const SubtypeInfoID &id = module.subtype()->id();
			const SharedPtr<SubtypeInfo> subtypeInfo = m_subtypeProvider->findById(id);

			if (subtypeInfo.isNull()) {
				throw IllegalStateException(
					"no such subtype " + id.toString() + " for device " + device);
			}

			copy.setSubtype(subtypeInfo);
		}

		result.add(copy);
	}

	return result;
}

void DeviceInfoProvider::loadInfo()
{
	parseFile<DevicesSAXHandler>(m_devicesFile, "device");

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
