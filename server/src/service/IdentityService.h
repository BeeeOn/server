#ifndef BEEEON_IDENTITY_SERVICE_H
#define BEEEON_IDENTITY_SERVICE_H

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class Identity;
class VerifiedIdentity;

class IdentityService {
public:
	typedef Poco::SharedPtr<IdentityService> Ptr;

	virtual ~IdentityService();

	virtual bool fetch(VerifiedIdentity &identity) = 0;
	virtual bool fetch(Identity &identity) = 0;
};

}

#endif
