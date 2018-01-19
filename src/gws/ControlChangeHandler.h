#pragma once

#include <Poco/Timestamp.h>

#include "dao/ControlDao.h"
#include "model/Control.h"
#include "model/Device.h"
#include "rpc/GatewayRPCHandler.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

/**
 * @brief Handler of device-pair operation state.
 */
class ControlChangeHandler : public GatewayRPCHandler, public Transactional {
public:
	typedef Poco::AutoPtr<ControlChangeHandler> Ptr;

	ControlChangeHandler(
		const Control &control,
		const Device &device,
		ControlDao::Ptr dao);
	~ControlChangeHandler();

	const Control &control() const;
	const Device &device() const;

	void onPending(GatewayRPCResult::Ptr result) override;
	void onAccepted(GatewayRPCResult::Ptr result) override;
	void onSuccess(GatewayRPCResult::Ptr result) override;
	void onFailed(GatewayRPCResult::Ptr result) override;
	void onNotConnected(GatewayRPCResult::Ptr result) override;
	void onTimeout(GatewayRPCResult::Ptr result) override;

protected:
	void finishRequest(bool failed);

private:
	Control m_control;
	Device m_device;
	ControlDao::Ptr m_controlDao;
	bool m_finished;
};

}
