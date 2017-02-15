#ifndef BEEEON_IDENTITY_DAO_H
#define BEEEON_IDENTITY_DAO_H

#include "model/Identity.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"

namespace BeeeOn {

class IdentityDao {
public:
	virtual void create(Identity &identity) = 0;
	virtual bool fetch(Identity &identity) = 0;
	virtual bool fetchBy(Identity &identity,
			const std::string &email) = 0;
	virtual bool remove(const Identity &identity) = 0;
};

class NullIdentityDao : public NullDao<Identity, IdentityDao> {
public:
	bool fetchBy(Identity &identity,
			const std::string &email)
	{
		return fetch(identity);
	}

	static IdentityDao &instance();
};

class MockIdentityDao : public MockDao<Identity, IdentityDao> {
public:
	void create(Identity &identity);
	bool fetchBy(Identity &identity,
			const std::string &email);
protected:
	IdentityID nextID()
	{
		return IdentityID::random();
	}

	Iterator findByEmail(const std::string &email);
};

}

#endif
