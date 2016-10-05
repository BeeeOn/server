#include "service/IdentityService.h"

BEEEON_OBJECT(IdentityService, BeeeOn::IdentityService)

using namespace BeeeOn;

IdentityService::IdentityService():
	m_identityDao(&NullIdentityDao::instance()),
	m_verifiedIdentityDao(&NullVerifiedIdentityDao::instance())
{
	injector<IdentityService, IdentityDao>(
		"identityDao",
		&IdentityService::setIdentityDao
	);
	injector<IdentityService, VerifiedIdentityDao>(
		"verifiedIdentityDao",
		&IdentityService::setVerifiedIdentityDao
	);
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

bool IdentityService::fetch(VerifiedIdentity &identity)
{
	return m_verifiedIdentityDao->fetch(identity);
}

bool IdentityService::fetch(Identity &identity)
{
	return m_identityDao->fetch(identity);
}
