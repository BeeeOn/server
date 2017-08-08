#ifndef BEEEON_IDENTITY_ACCESS_POLICY_H
#define BEEEON_IDENTITY_ACCESS_POLICY_H

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class Identity;
class VerifiedIdentity;
class PolicyContext;

class IdentityAccessPolicy {
public:
	typedef Poco::SharedPtr<IdentityAccessPolicy> Ptr;

	enum Action {
		ACTION_USER_GET,
	};

	virtual ~IdentityAccessPolicy();

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const Identity &identity) = 0;

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const VerifiedIdentity &identity) = 0;
};

}

#endif
