#ifndef BEEEON_IDENTITY_SERVICE_IMPL_H
#define BEEEON_IDENTITY_SERVICE_IMPL_H

#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "service/IdentityService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class IdentityServiceImpl : public IdentityService, public Transactional {
public:
	IdentityServiceImpl();

	void setIdentityDao(IdentityDao *dao);
	void setVerifiedIdentityDao(VerifiedIdentityDao *dao);

	bool fetch(VerifiedIdentity &identity)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(identity));
	}

	bool fetch(Identity &identity)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(identity));
	}

protected:
	bool doFetch(VerifiedIdentity &identity);
	bool doFetch(Identity &identity);

private:
	IdentityDao *m_identityDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
};

}

#endif

