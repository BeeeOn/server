#ifndef BEEEON_POLICY_CONTEXT_H
#define BEEEON_POLICY_CONTEXT_H

#include "util/Castable.h"

namespace BeeeOn {

class User;

/**
 * Interface providing a context to the policy framework.
 */
class PolicyContext : public Castable {
public:
	virtual ~PolicyContext();

	virtual const User &user() const = 0;
};

class UserPolicyContext : public PolicyContext {
public:
	UserPolicyContext(User &user);
	UserPolicyContext();

	void setUser(User &user);
	const User &user() const override;

private:
	User *m_user;
};

}

#endif
