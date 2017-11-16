#ifndef BEEEON_VERIFIED_IDENTITY_DAO_H
#define BEEEON_VERIFIED_IDENTITY_DAO_H

#include <list>
#include <vector>

#include <Poco/SharedPtr.h>

#include "dao/MockDao.h"
#include "model/VerifiedIdentity.h"

namespace BeeeOn {

class VerifiedIdentityDao {
public:
	typedef Poco::SharedPtr<VerifiedIdentityDao> Ptr;

	virtual ~VerifiedIdentityDao();

	virtual void create(VerifiedIdentity &identity) = 0;
	virtual bool fetch(VerifiedIdentity &identity) = 0;
	virtual bool fetchBy(VerifiedIdentity &identity,
			const std::string email,
			const std::string provider) = 0;
	virtual void fetchBy(std::vector<VerifiedIdentity> &identities,
			const std::string email) = 0;
	virtual void fetchBy(std::list<VerifiedIdentity> &identities,
			const User &user) = 0;
	virtual bool update(VerifiedIdentity &identity) = 0;
	virtual bool remove(const VerifiedIdentity &identity) = 0;
};

class MockVerifiedIdentityDao : public MockDao<VerifiedIdentity, VerifiedIdentityDao> {
public:
	bool fetchBy(VerifiedIdentity &identity,
			const std::string email,
			const std::string provider);

	void fetchBy(std::vector<VerifiedIdentity> &identities,
			const std::string email);

	void fetchBy(std::list<VerifiedIdentity> &identities,
			const User &user);

protected:
	VerifiedIdentityID nextID()
	{
		return VerifiedIdentityID::random();
	}
};

}

#endif
