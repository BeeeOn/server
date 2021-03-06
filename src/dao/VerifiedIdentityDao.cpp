#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "di/Injectable.h"
#include "dao/VerifiedIdentityDao.h"

BEEEON_OBJECT_BEGIN(BeeeOn, MockVerifiedIdentityDao)
BEEEON_OBJECT_CASTABLE(VerifiedIdentityDao)
BEEEON_OBJECT_END(BeeeOn, MockVerifiedIdentityDao)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

VerifiedIdentityDao::~VerifiedIdentityDao()
{
}

bool MockVerifiedIdentityDao::fetchBy(VerifiedIdentity &identity,
			const std::string email,
			const std::string provider)
{
	Iterator it = storage().begin();

	for (; it != storage().end(); ++it) {
		const VerifiedIdentity &tmp = *it->second;

		if (tmp.identity().email() != email)
			continue;

		if (tmp.provider() != provider)
			continue;

		identity = tmp;
		return true;
	}

	return false;
}

void MockVerifiedIdentityDao::fetchBy(
		vector<VerifiedIdentity> &identities,
		const string email)
{
	Iterator it = storage().begin();

	for (; it != storage().end(); ++it) {
		const VerifiedIdentity &tmp = *it->second;

		if (tmp.email() != email)
			continue;

		identities.push_back(tmp);
	}
}

void MockVerifiedIdentityDao::fetchBy(
		list<VerifiedIdentity> &identities,
		const User &user)
{
	Iterator it = storage().begin();

	for (; it != storage().end(); ++it) {
		const VerifiedIdentity &tmp = *it->second;

		if (tmp.user().id() != user.id())
			continue;

		identities.push_back(tmp);
	}
}
