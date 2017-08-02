#ifndef BEEEON_GWS_DEVICE_SERVICE_IMPL_H
#define BEEEON_GWS_DEVICE_SERVICE_IMPL_H

#include "dao/DeviceDao.h"
#include "provider/DeviceInfoProvider.h"
#include "service/GWSDeviceService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class GWSDeviceServiceImpl : public GWSDeviceService, public Transactional {
public:
	void setDeviceDao(DeviceDao::Ptr dao);
	void setDeviceInfoProvider(DeviceInfoProvider::Ptr provider);

	bool registerDevice(Device &device, const Gateway &gateway) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doRegisterDevice(device, gateway));
	}

	void fetchActiveWithPrefix(std::vector<Device> &devices,
			const Gateway &gateway,
			const DevicePrefix &prefix) override
	{
		return BEEEON_TRANSACTION(doFetchActiveWithPrefix(devices, gateway, prefix));
	}

protected:
	bool doRegisterDevice(Device &device, const Gateway &gateway);

	void doFetchActiveWithPrefix(std::vector<Device> &devices,
			const Gateway &gateway,
			const DevicePrefix &prefix);

private:
	DeviceDao::Ptr m_deviceDao;
	DeviceInfoProvider::Ptr m_deviceInfoProvider;
};

}

#endif
