#ifndef BEEEON_DEVICE_INFO_PROVIDER_H
#define BEEEON_DEVICE_INFO_PROVIDER_H

#include <set>

#include "di/AbstractInjectorTarget.h"
#include "model/ModuleInfo.h"
#include "model/DeviceInfo.h"
#include "provider/InfoProvider.h"
#include "util/DevicesSAXHandler.h"

namespace BeeeOn {

class DeviceInfoProvider :
		public AbstractInjectorTarget,
		public XmlInfoProvider<DeviceInfo, DevicesSAXHandler> {
public:
	DeviceInfoProvider();

	void setTypeInfoProvider(InfoProvider<TypeInfo> *provider);
	void setDevicesFile(const std::string &devicesFile);

protected:
	DeviceInfo resolveTypes(const DeviceInfo &device);
	void injectionDone() override;

private:
	std::string m_devicesFile;
	InfoProvider<TypeInfo> *m_typeProvider;
};

}

#endif
