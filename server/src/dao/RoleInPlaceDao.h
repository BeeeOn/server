#ifndef BEEEON_ROLE_IN_PLACE_DAO_H
#define BEEEON_ROLE_IN_PLACE_DAO_H

#include "di/InjectorTarget.h"
#include "model/RoleInPlace.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"

namespace BeeeOn {

class RoleInPlaceDao {
public:
	virtual void create(RoleInPlace &role) = 0;
	virtual bool fetch(RoleInPlace &role) = 0;
	virtual bool update(RoleInPlace &role) = 0;
	virtual bool remove(const RoleInPlace &role) = 0;

	virtual AccessLevel fetchAccessLevel(
			const Place &place,
			const User &user) = 0;
};

class NullRoleInPlaceDao : public AbstractInjectorTarget,
		public NullDao<RoleInPlace, RoleInPlaceDao> {
public:
	AccessLevel fetchAccessLevel(
			const Place &place,
			const User &user)
	{
		throw Poco::NotImplementedException(__func__);
	}

	static RoleInPlaceDao &instance();
};

class MockRoleInPlaceDao : public AbstractInjectorTarget,
		public MockDao<RoleInPlace, RoleInPlaceDao> {
public:
	AccessLevel fetchAccessLevel(
			const Place &place,
			const User &user);
protected:
	RoleInPlaceID nextID()
	{
		return RoleInPlaceID::random();
	}
};

}

#endif
