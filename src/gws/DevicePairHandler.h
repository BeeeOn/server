#pragma once

#include "dao/DeviceDao.h"
#include "model/Device.h"
#include "rpc/GatewayRPCHandler.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

/**
 * @brief Handler of device-pair operation state.
 */
class DevicePairHandler : public GatewayRPCHandler, public Transactional {
public:
	typedef Poco::AutoPtr<DevicePairHandler> Ptr;

	DevicePairHandler(const Device &device, DeviceDao::Ptr dao);

	void onAny(GatewayRPCResult::Ptr r) override;

private:
	Device m_device;
	DeviceDao::Ptr m_deviceDao;
};

}

