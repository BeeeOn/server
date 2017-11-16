#ifndef BEEEON_IDENTITY_SERVICE_H
#define BEEEON_IDENTITY_SERVICE_H

#include <list>

#include <Poco/SharedPtr.h>

#include "service/Relation.h"
#include "service/Single.h"

namespace BeeeOn {

class Identity;
class User;
class VerifiedIdentity;

class IdentityService {
public:
	typedef Poco::SharedPtr<IdentityService> Ptr;

	virtual ~IdentityService();

	virtual bool fetch(Single<VerifiedIdentity> &identity) = 0;
	virtual bool fetch(Single<Identity> &identity) = 0;
	virtual void list(Relation<std::list<VerifiedIdentity>, User> &input) = 0;
};

}

#endif
