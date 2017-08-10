#include "di/Injectable.h"
#include "service/IdentityServiceImpl.h"

BEEEON_OBJECT_BEGIN(BeeeOn, IdentityServiceImpl)
BEEEON_OBJECT_CASTABLE(IdentityService)
BEEEON_OBJECT_REF("identityDao", &IdentityServiceImpl::setIdentityDao)
BEEEON_OBJECT_REF("verifiedIdentityDao", &IdentityServiceImpl::setVerifiedIdentityDao)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, IdentityServiceImpl)

using namespace BeeeOn;

IdentityServiceImpl::IdentityServiceImpl()
{
}

void IdentityServiceImpl::setIdentityDao(IdentityDao::Ptr dao)
{
	m_identityDao = dao;
}

void IdentityServiceImpl::setVerifiedIdentityDao(VerifiedIdentityDao::Ptr dao)
{
	m_verifiedIdentityDao = dao;
}

bool IdentityServiceImpl::doFetch(VerifiedIdentity &identity)
{
	return m_verifiedIdentityDao->fetch(identity);
}

bool IdentityServiceImpl::doFetch(Identity &identity)
{
	return m_identityDao->fetch(identity);
}
