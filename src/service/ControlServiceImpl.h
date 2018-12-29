#pragma once

#include <Poco/SharedPtr.h>

#include "dao/ControlDao.h"
#include "dao/DeviceDao.h"
#include "gws/ControlChangeHandler.h"
#include "policy/ControlAccessPolicy.h"
#include "rpc/GatewayRPC.h"
#include "service/ControlService.h"
#include "transaction/Transactional.h"
#include "util/UnknownEvaluator.h"

namespace BeeeOn {

class ControlServiceImpl : public ControlService, public Transactional {
public:
	typedef Poco::SharedPtr<ControlServiceImpl> Ptr;

	void setDeviceDao(DeviceDao::Ptr dao);
	void setControlDao(ControlDao::Ptr dao);
	void setGatewayRPC(GatewayRPC::Ptr rpc);
	void setAccessPolicy(ControlAccessPolicy::Ptr policy);

	bool fetch(Relation<Control, Device> &data) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(data));
	}

	void fetchMany(Relation<std::list<Control>, Device> &data) override
	{
		BEEEON_TRANSACTION(doFetchMany(data));
	}

	void requestChange(Relation<Control, Device> &data,
			double value,
			const Poco::Timespan &timeout,
			const OpMode &mode,
			bool force) override
	{
		ControlChangeHandler::Ptr handler =
			BEEEON_TRANSACTION_RETURN(
				ControlChangeHandler::Ptr,
				doRequestChange(data, value, timeout, force));

		if (handler.isNull())
			return;

		const ModuleInfo &info = handler->control().info();
		double raw = value;

		if (!info.toUnknown().empty())
			raw = m_unknownEvaluator.toRaw(info, value);

		m_gatewayRPC->updateActor(
				handler,
				handler->device().gateway(),
				handler->device(),
				handler->control().info(),
				raw,
				timeout,
				mode);
	}

	void fixupControls();

protected:
	bool doFetch(Relation<Control, Device> &data);
	void doFetchMany(Relation<std::list<Control>, Device> &data);
	ControlChangeHandler::Ptr doRequestChange(
			Relation<Control, Device> &data,
			double value,
			const Poco::Timespan &timeout,
			bool force);

	void startRequest(
			const UserPolicyContext &policyContext,
			const Device &device,
			Control &control,
			double value);
	void cancelRequest(
			const Device &device,
			Control &control);

private:
	DeviceDao::Ptr m_deviceDao;
	ControlDao::Ptr m_controlDao;
	GatewayRPC::Ptr m_gatewayRPC;
	ControlAccessPolicy::Ptr m_accessPolicy;
	UnknownEvaluator m_unknownEvaluator;
};

}
