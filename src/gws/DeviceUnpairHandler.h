#pragma once

#include "dao/DeviceDao.h"
#include "gws/DeviceListener.h"
#include "model/Device.h"
#include "rpc/GatewayRPCHandler.h"
#include "rpc/GatewayRPCUnpairResult.h"
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

	/**
	 * @brief On success, we consider the type of received result.
	 * If it is a generic success result, the unpairing works in the
	 * legacy way - the device requested to be unpaired is unpaired.
	 * If the result contains a set of unpaired devices, we unpair
	 * those devices.
	 *
	 * In case when the device requested to be unpaired was not unpaired
	 * but other devices were, the DeviceListener::onUnpairedFailed event
	 * is generated for the requested device while for all the confirmed
	 * devices, the DeviceListener::onUnpairedConfirmed event is emitted.
	 */
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
