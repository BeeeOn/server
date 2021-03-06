#pragma once

#include <Poco/SharedPtr.h>

#include "model/Identity.h"
#include "dao/MockDao.h"

namespace BeeeOn {

class IdentityDao {
public:
	typedef Poco::SharedPtr<IdentityDao> Ptr;

	virtual ~IdentityDao();

	virtual void create(Identity &identity) = 0;
	virtual bool fetch(Identity &identity) = 0;
	virtual bool fetchBy(Identity &identity,
			const std::string &email) = 0;
	virtual bool remove(const Identity &identity) = 0;
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
