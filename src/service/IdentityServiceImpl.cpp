#include "di/Injectable.h"
#include "service/IdentityServiceImpl.h"

BEEEON_OBJECT_BEGIN(BeeeOn, IdentityServiceImpl)
BEEEON_OBJECT_CASTABLE(IdentityService)
BEEEON_OBJECT_REF("identityDao", &IdentityServiceImpl::setIdentityDao)
BEEEON_OBJECT_REF("verifiedIdentityDao", &IdentityServiceImpl::setVerifiedIdentityDao)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_REF("accessPolicy", &IdentityServiceImpl::setAccessPolicy)
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

void IdentityServiceImpl::setAccessPolicy(IdentityAccessPolicy::Ptr accessPolicy)
{
	m_accessPolicy = accessPolicy;
}

bool IdentityServiceImpl::doFetch(Single<VerifiedIdentity> &input)
{
	m_accessPolicy->assure(IdentityAccessPolicy::ACTION_USER_GET, input, input.target());
	return m_verifiedIdentityDao->fetch(input.target());
}

bool IdentityServiceImpl::doFetch(Single<Identity> &input)
{
	m_accessPolicy->assure(IdentityAccessPolicy::ACTION_USER_GET, input, input.target());
	return m_identityDao->fetch(input.target());
}

void IdentityServiceImpl::doList(Relation<std::list<VerifiedIdentity>, User> &input)
{
	m_accessPolicy->assure(IdentityAccessPolicy::ACTION_USER_GET, input, input.base());
	m_verifiedIdentityDao->fetchBy(input.target(), input.base());
}
