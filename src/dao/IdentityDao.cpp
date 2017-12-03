#include <Poco/SingletonHolder.h>

#include "di/Injectable.h"
#include "dao/IdentityDao.h"

BEEEON_OBJECT_BEGIN(BeeeOn, MockIdentityDao)
BEEEON_OBJECT_CASTABLE(IdentityDao)
BEEEON_OBJECT_END(BeeeOn, MockIdentityDao)

using namespace Poco;
using namespace BeeeOn;

IdentityDao::~IdentityDao()
{
}

MockIdentityDao::Iterator MockIdentityDao::findByEmail(
		const std::string &email)
{
	Iterator it;

	for (it = storage().begin(); it != storage().end(); ++it) {
		if (it->second->email() != email)
			continue;

		return it;
	}

	return storage().end();
}

void MockIdentityDao::create(Identity &identity)
{
	Iterator it = findByEmail(identity.email());

	if (it != storage().end()) {
		throw ExistsException("identity ("
				+ identity.email()
				+ ") already exists for "
				+ it->second->id().toString());
	}

	MockDao<Identity, IdentityDao>::create(identity);
}

bool MockIdentityDao::fetchBy(Identity &identity,
		const std::string &email)
{
	Iterator it = findByEmail(email);

	if (it == storage().end())
		return false;

	identity = *it->second;
	return true;
}
