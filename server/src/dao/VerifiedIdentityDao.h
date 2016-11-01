#ifndef BEEEON_VERIFIED_IDENTITY_DAO_H
#define BEEEON_VERIFIED_IDENTITY_DAO_H

#include <vector>

#include "di/InjectorTarget.h"
#include "dao/IdentityDao.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "model/VerifiedIdentity.h"

namespace BeeeOn {

class VerifiedIdentityDao {
public:
	virtual void create(VerifiedIdentity &identity) = 0;
	virtual bool fetch(VerifiedIdentity &identity) = 0;
	virtual bool fetchBy(VerifiedIdentity &identity,
			const std::string email,
			const std::string provider) = 0;
	virtual void fetchBy(std::vector<VerifiedIdentity> &identities,
			const std::string email) = 0;
	virtual bool update(VerifiedIdentity &identity) = 0;
	virtual bool remove(const VerifiedIdentity &identity) = 0;
};

class NullVerifiedIdentityDao : public AbstractInjectorTarget,
		public NullDao<VerifiedIdentity, VerifiedIdentityDao> {
public:
	virtual bool fetchBy(VerifiedIdentity &identity,
			const std::string email,
			const std::string provider)
	{
		return fetch(identity);
	}

	void fetchBy(std::vector<VerifiedIdentity> &identities,
			const std::string email);

	static VerifiedIdentityDao &instance();
};

class MockVerifiedIdentityDao : public AbstractInjectorTarget,
		public MockDao<VerifiedIdentity, VerifiedIdentityDao> {
public:
	bool fetchBy(VerifiedIdentity &identity,
			const std::string email,
			const std::string provider);

	void fetchBy(std::vector<VerifiedIdentity> &identities,
			const std::string email);

protected:
	VerifiedIdentityID nextID()
	{
		return VerifiedIdentityID::random();
	}
};

}

#endif
