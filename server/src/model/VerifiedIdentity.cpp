#include "model/VerifiedIdentity.h"

using namespace BeeeOn;

VerifiedIdentity::VerifiedIdentity()
{
}

VerifiedIdentity::VerifiedIdentity(const ID &id):
	m_id(id)
{
}

VerifiedIdentity::VerifiedIdentity(const VerifiedIdentity &copy):
	m_id(copy.m_id),
	m_provider(copy.m_provider),
	m_accessToken(copy.m_accessToken),
	m_picture(copy.m_picture),
	m_user(copy.m_user),
	m_identity(copy.m_identity)
{
}

VerifiedIdentity::VerifiedIdentity(const ID &id,
		const VerifiedIdentity &copy):
	m_id(id),
	m_provider(copy.m_provider),
	m_accessToken(copy.m_accessToken),
	m_picture(copy.m_picture),
	m_user(copy.m_user),
	m_identity(copy.m_identity)
{
}
