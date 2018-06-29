#pragma once

#include "dao/DeviceDao.h"
#include "gws/DeviceListener.h"
#include "model/Device.h"
#include "rpc/GatewayRPCHandler.h"
#include "transaction/Transactional.h"
#include "util/EventSource.h"

namespace BeeeOn {

/**
 * @brief Handler of device-pair operation state.
 */
class DevicePairHandler : public GatewayRPCHandler, public Transactional {
public:
	typedef Poco::AutoPtr<DevicePairHandler> Ptr;

	DevicePairHandler(
		const Device &device,
		DeviceDao::Ptr dao,
		Poco::SharedPtr<EventSource<DeviceListener>> source);

	void onPending(GatewayRPCResult::Ptr result) override;
	void onAccepted(GatewayRPCResult::Ptr result) override;
	void onSuccess(GatewayRPCResult::Ptr result) override;
	void onFailed(GatewayRPCResult::Ptr result) override;
	void onNotConnected(GatewayRPCResult::Ptr result) override;
	void onTimeout(GatewayRPCResult::Ptr result) override;

private:
	Device m_device;
	DeviceDao::Ptr m_deviceDao;
	Poco::SharedPtr<EventSource<DeviceListener>> m_eventSource;
};

}

