#ifndef BEEEON_IDENTITY_SERVICE_IMPL_H
#define BEEEON_IDENTITY_SERVICE_IMPL_H

#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "policy/IdentityAccessPolicy.h"
#include "service/IdentityService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class IdentityServiceImpl : public IdentityService, public Transactional {
public:
	IdentityServiceImpl();

	void setIdentityDao(IdentityDao::Ptr dao);
	void setVerifiedIdentityDao(VerifiedIdentityDao::Ptr dao);
	void setAccessPolicy(IdentityAccessPolicy::Ptr accessPolicy);

	bool fetch(Single<VerifiedIdentity> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	bool fetch(Single<Identity> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	void list(Relation<std::list<VerifiedIdentity>, User> &input) override
	{
		BEEEON_TRANSACTION(doList(input));
	}

protected:
	bool doFetch(Single<VerifiedIdentity> &input);
	bool doFetch(Single<Identity> &input);
	void doList(Relation<std::list<VerifiedIdentity>, User> &input);

private:
	IdentityDao::Ptr m_identityDao;
	VerifiedIdentityDao::Ptr m_verifiedIdentityDao;
	IdentityAccessPolicy::Ptr m_accessPolicy;
};

}

#endif

