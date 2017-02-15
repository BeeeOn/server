#ifndef BEEEON_DEVICE_INFO_PROVIDER_H
#define BEEEON_DEVICE_INFO_PROVIDER_H

#include <set>

#include "model/ModuleInfo.h"
#include "model/DeviceInfo.h"
#include "provider/InfoProvider.h"
#include "util/DevicesSAXHandler.h"

namespace BeeeOn {

class DeviceInfoProvider : public XmlInfoProvider<DeviceInfo, DevicesSAXHandler> {
public:
	DeviceInfoProvider();

	void setTypeInfoProvider(InfoProvider<TypeInfo> *provider);
	void setDevicesFile(const std::string &devicesFile);
	void loadInfo();

protected:
	DeviceInfo resolveTypes(const DeviceInfo &device);

private:
	std::string m_devicesFile;
	InfoProvider<TypeInfo> *m_typeProvider;
};

}

#endif
