#include "di/Injectable.h"
#include "service/IdentityService.h"

BEEEON_OBJECT_BEGIN(BeeeOn, IdentityService)
BEEEON_OBJECT_REF("identityDao", &IdentityService::setIdentityDao)
BEEEON_OBJECT_REF("verifiedIdentityDao", &IdentityService::setVerifiedIdentityDao)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, IdentityService)

using namespace BeeeOn;

IdentityService::IdentityService():
	m_identityDao(&NullIdentityDao::instance()),
	m_verifiedIdentityDao(&NullVerifiedIdentityDao::instance())
{
}

void IdentityService::setIdentityDao(IdentityDao *dao)
{
	if (dao == NULL)
		m_identityDao = &NullIdentityDao::instance();
	else
		m_identityDao = dao;
}

void IdentityService::setVerifiedIdentityDao(VerifiedIdentityDao *dao)
{
	if (dao == NULL) {
		m_verifiedIdentityDao =
			&NullVerifiedIdentityDao::instance();
	}
	else {
		m_verifiedIdentityDao = dao;
	}
}

bool IdentityService::doFetch(VerifiedIdentity &identity)
{
	return m_verifiedIdentityDao->fetch(identity);
}

bool IdentityService::doFetch(Identity &identity)
{
	return m_identityDao->fetch(identity);
}
