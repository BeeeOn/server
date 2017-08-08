#include <Poco/Exception.h>

#include "policy/PolicyContext.h"

using namespace Poco;
using namespace BeeeOn;

UserPolicyContext::UserPolicyContext(User &user):
	m_user(&user)
{
}

UserPolicyContext::UserPolicyContext():
	m_user(0)
{
}

void UserPolicyContext::setUser(User &user)
{
	m_user = &user;
}

const User &UserPolicyContext::user() const
{
	if (m_user)
		return *m_user;

	throw IllegalStateException("no user has been specified");
}
