#pragma once

#include "dao/GatewayMessageDao.h"
#include "gws/DeviceListener.h"
#include "policy/GatewayMessageAccessPolicy.h"
#include "service/GatewayMessageService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class GatewayMessageServiceImpl :
	public GatewayMessageService,
	public DeviceListener,
	public Transactional {
public:
	GatewayMessageServiceImpl();

	void setAccessPolicy(GatewayMessageAccessPolicy::Ptr policy);
	void setGatewayMessageDao(GatewayMessageDao::Ptr dao);
	void setGatewayMessagesLimit(int limit);

	bool fetch(Relation<GatewayMessage, Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	void fetchMany(Relation<std::vector<GatewayMessage>, Gateway> &input) override
	{
		BEEEON_TRANSACTION(doFetchMany(input));
	}

	bool remove(Relation<GatewayMessage, Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doRemove(input));
	}

	void onRefusedNewDevice(const DeviceEvent &e) override;
	void onPairFailed(const DeviceEvent &e) override;
	void onUnpairFailed(const DeviceEvent &e) override;

protected:
	bool doFetch(Relation<GatewayMessage, Gateway> &input);
	void doFetchMany(Relation<std::vector<GatewayMessage>, Gateway> &input);
	bool doRemove(Relation<GatewayMessage, Gateway> &input);

private:
	GatewayMessageAccessPolicy::Ptr m_policy;
	GatewayMessageDao::Ptr m_dao;
	size_t m_gatewayMessagesLimit;
};

}
