#pragma once

#include "dao/DeviceDao.h"
#include "gws/DeviceListener.h"
#include "model/Device.h"
#include "rpc/GatewayRPCHandler.h"
#include "transaction/Transactional.h"
#include "util/EventSource.h"

namespace BeeeOn {

/**
 * @brief Handle the device-unpair operation states.
 */
class DeviceUnpairHandler : public GatewayRPCHandler, public Transactional {
public:
	typedef Poco::AutoPtr<DeviceUnpairHandler> Ptr;

	DeviceUnpairHandler(
		const Device &device,
		DeviceDao::Ptr dao,
		Poco::SharedPtr<EventSource<DeviceListener>> source);

	void onSuccess(GatewayRPCResult::Ptr r) override;
	void onAny(GatewayRPCResult::Ptr r) override;

protected:
	void deviceUnpaired(Device &device);

private:
	Device m_device;
	DeviceDao::Ptr m_deviceDao;
	Poco::SharedPtr<EventSource<DeviceListener>> m_eventSource;
};

}
