#ifndef BEEEON_DEVICE_INFO_PROVIDER_H
#define BEEEON_DEVICE_INFO_PROVIDER_H

#include <set>

#include <Poco/SharedPtr.h>

#include "model/ModuleInfo.h"
#include "model/DeviceInfo.h"
#include "model/SubtypeInfo.h"
#include "provider/InfoProvider.h"

namespace BeeeOn {

class DeviceInfoProvider : public XmlInfoProvider<DeviceInfo> {
public:
	typedef Poco::SharedPtr<DeviceInfoProvider> Ptr;

	DeviceInfoProvider();

	void setTypeInfoProvider(InfoProvider<TypeInfo> *provider);
	void setSubtypeInfoProvider(InfoProvider<SubtypeInfo> *provider);
	void setDevicesFile(const std::string &devicesFile);
	void loadInfo();

	const Poco::SharedPtr<DeviceInfo> findByNameAndVendor(
			const std::string &name,
			const std::string &vendor) const;

protected:
	DeviceInfo resolveTypes(const DeviceInfo &device);

private:
	std::string m_devicesFile;
	InfoProvider<TypeInfo> *m_typeProvider;
	InfoProvider<SubtypeInfo> *m_subtypeProvider;
};

}

#endif
