#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "dao/VerifiedIdentityDao.h"

BEEEON_OBJECT(BeeeOn, MockVerifiedIdentityDao)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

VerifiedIdentityDao &NullVerifiedIdentityDao::instance()
{
	static Poco::SingletonHolder<NullVerifiedIdentityDao> singleton;
	return *singleton.get();
}

void NullVerifiedIdentityDao::fetchBy(
		vector<VerifiedIdentity> &identities,
		const string email)
{
	throw NotImplementedException(__func__);
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
