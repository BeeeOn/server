#pragma once

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class Identity;
class VerifiedIdentity;
class User;
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

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const User &user) = 0;
};

}
