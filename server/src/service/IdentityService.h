#ifndef BEEEON_IDENTITY_SERVICE_H
#define BEEEON_IDENTITY_SERVICE_H

#include "di/InjectorTarget.h"
#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"

namespace BeeeOn {

class IdentityService : public AbstractInjectorTarget {
public:
	IdentityService();

	void setIdentityDao(IdentityDao *dao);
	void setVerifiedIdentityDao(VerifiedIdentityDao *dao);

	bool fetch(VerifiedIdentity &identity);
	bool fetch(Identity &identity);

private:
	IdentityDao *m_identityDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
};

}

#endif
