#pragma once

#include "dao/FCMTokenDao.h"
#include "policy/FCMTokenAccessPolicy.h"
#include "service/FCMTokenService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class FCMTokenServiceImpl : public FCMTokenService, public Transactional {
public:
	FCMTokenServiceImpl();

	void setFCMTokenDao(FCMTokenDao::Ptr dao);
	void setAccessPolicy(FCMTokenAccessPolicy::Ptr accessPolicy);

	void registerToken(Single<FCMToken> &input) override
	{
		BEEEON_TRANSACTION(doRegisterToken(input));
	}

	void unregisterToken(Single<FCMToken> &input) override
	{
		BEEEON_TRANSACTION(doUnregisterToken(input));
	}

protected:
	void doRegisterToken(Single<FCMToken> &input);
	void doUnregisterToken(Single<FCMToken> &input);

private:
	FCMTokenDao::Ptr m_dao;
	FCMTokenAccessPolicy::Ptr m_accessPolicy;
};

}
