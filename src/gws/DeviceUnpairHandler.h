#pragma once

#include "dao/DeviceDao.h"
#include "model/Device.h"
#include "rpc/GatewayRPCHandler.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

/**
 * @brief Handle the device-unpair operation states.
 */
class DeviceUnpairHandler : public GatewayRPCHandler, public Transactional {
public:
	typedef Poco::AutoPtr<DeviceUnpairHandler> Ptr;

	DeviceUnpairHandler(const Device &device, DeviceDao::Ptr dao);

	void onAny(GatewayRPCResult::Ptr r) override;

private:
	Device m_device;
	DeviceDao::Ptr m_deviceDao;
};

}
