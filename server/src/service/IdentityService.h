#ifndef BEEEON_IDENTITY_SERVICE_H
#define BEEEON_IDENTITY_SERVICE_H

#include <Poco/SharedPtr.h>

#include "service/Single.h"

namespace BeeeOn {

class Identity;
class VerifiedIdentity;

class IdentityService {
public:
	typedef Poco::SharedPtr<IdentityService> Ptr;

	virtual ~IdentityService();

	virtual bool fetch(Single<VerifiedIdentity> &identity) = 0;
	virtual bool fetch(Single<Identity> &identity) = 0;
};

}

#endif
