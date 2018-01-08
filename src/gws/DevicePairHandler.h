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

	void onPending(GatewayRPCResult::Ptr result) override;
	void onAccepted(GatewayRPCResult::Ptr result) override;
	void onSuccess(GatewayRPCResult::Ptr result) override;
	void onFailed(GatewayRPCResult::Ptr result) override;
	void onNotConnected(GatewayRPCResult::Ptr result) override;
	void onTimeout(GatewayRPCResult::Ptr result) override;

private:
	Device m_device;
	DeviceDao::Ptr m_deviceDao;
};

}

