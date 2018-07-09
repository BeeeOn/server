#include "service/IdentityListener.h"

using namespace std;
using namespace BeeeOn;

IdentityInviteEvent::IdentityInviteEvent()
{
}

void IdentityInviteEvent::setOriginator(const User &originator)
{
	m_originator = originator;
}

const User &IdentityInviteEvent::originator() const
{
	return m_originator;
}

void IdentityInviteEvent::setGateway(const Gateway &gateway)
{
	m_gateway = gateway;
}

const Gateway &IdentityInviteEvent::gateway() const
{
	return m_gateway;
}

void IdentityInviteEvent::setIdentity(const Identity &identity)
{
	m_identity = identity;
}

const Identity &IdentityInviteEvent::identity() const
{
	return m_identity;
}

void IdentityInviteEvent::setLevel(const AccessLevel &level)
{
	m_level = level;
}

const AccessLevel &IdentityInviteEvent::level() const
{
	return m_level;
}

VerifiedIdentityEvent::VerifiedIdentityEvent()
{
}

void VerifiedIdentityEvent::setVerifiedIdentity(const VerifiedIdentity &identity)
{
	m_identity = identity;
}

const VerifiedIdentity &VerifiedIdentityEvent::verifiedIdentity() const
{
	return m_identity;
}

IdentityListener::~IdentityListener()
{
}

void IdentityListener::onFirstLogin(const VerifiedIdentityEvent &e)
{
}

void IdentityListener::onInvite(const IdentityInviteEvent &e)
{
}
