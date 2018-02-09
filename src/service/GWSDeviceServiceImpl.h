#ifndef BEEEON_GWS_DEVICE_SERVICE_IMPL_H
#define BEEEON_GWS_DEVICE_SERVICE_IMPL_H

#include <list>

#include <Poco/SharedPtr.h>

#include "dao/DeviceDao.h"
#include "model/ModuleType.h"
#include "provider/DeviceInfoProvider.h"
#include "service/GWSDeviceService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class GWSDeviceServiceImpl : public GWSDeviceService, public Transactional {
public:
	void setDeviceDao(DeviceDao::Ptr dao);
	void setDeviceInfoProvider(DeviceInfoProvider::Ptr provider);

	bool registerDevice(Device &device,
			const std::string &name,
			const std::string &vendor,
			const Gateway &gateway) override
	{
		return BEEEON_TRANSACTION_RETURN(bool,
			doRegisterDevice(device, name, vendor, gateway));
	}

	void fetchActiveWithPrefix(std::vector<Device> &devices,
			const Gateway &gateway,
			const DevicePrefix &prefix) override
	{
		return BEEEON_TRANSACTION(doFetchActiveWithPrefix(devices, gateway, prefix));
	}

protected:
	bool doRegisterDevice(Device &device,
			const std::string &name,
			const std::string &vendor,
			const Gateway &gateway);

	void doFetchActiveWithPrefix(std::vector<Device> &devices,
			const Gateway &gateway,
			const DevicePrefix &prefix);

private:
	/**
	 * @brief Verify that count of given modules is less or equals to DeviceInfo
	 * specification and types of modules are identical.
	 *
	 * @throw InvalidArgumentException for invalid count of modules.
	 * @throw MultiException for invalid types of modules.
	 */
	void verifyModules(const Poco::SharedPtr<DeviceInfo> deviceInfo,
			const std::list<ModuleType> &modules) const;

	DeviceDao::Ptr m_deviceDao;
	DeviceInfoProvider::Ptr m_deviceInfoProvider;
};

}

#endif
