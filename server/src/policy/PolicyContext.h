#ifndef BEEEON_POLICY_CONTEXT_H
#define BEEEON_POLICY_CONTEXT_H

#include <Poco/Exception.h>

namespace BeeeOn {

class User;

/**
 * Interface providing a context to the policy framework.
 */
class PolicyContext {
public:
	virtual ~PolicyContext();

	virtual const User &user() const = 0;
};

class UserPolicyContext : public PolicyContext {
public:
	UserPolicyContext(User &user):
		m_user(&user)
	{
	}

	UserPolicyContext():
		m_user(0)
	{
	}

	void setUser(User &user)
	{
		m_user = &user;
	}

	const User &user() const override
	{
		if (m_user)
			return *m_user;

		throw Poco::IllegalStateException(
				"no user has been specified");
	}

private:
	User *m_user;
};

}

#endif
