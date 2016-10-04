#include "dao/VerifiedIdentityDao.h"

BEEEON_OBJECT(MockVerifiedIdentityDao, BeeeOn::MockVerifiedIdentityDao)

using namespace BeeeOn;

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
