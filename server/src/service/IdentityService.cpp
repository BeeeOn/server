#include "service/IdentityService.h"

BEEEON_OBJECT(BeeeOn, IdentityService)

using namespace BeeeOn;

IdentityService::IdentityService():
	m_identityDao(&NullIdentityDao::instance()),
	m_verifiedIdentityDao(&NullVerifiedIdentityDao::instance())
{
	injector("identityDao", &IdentityService::setIdentityDao);
	injector("verifiedIdentityDao", &IdentityService::setVerifiedIdentityDao);
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
