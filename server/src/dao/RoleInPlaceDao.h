#ifndef BEEEON_ROLE_IN_PLACE_DAO_H
#define BEEEON_ROLE_IN_PLACE_DAO_H

#include <vector>

#include "di/InjectorTarget.h"
#include "model/RoleInPlace.h"
#include "dao/NullDao.h"

namespace BeeeOn {

class User;
class VerifiedIdentityDao;

class RoleInPlaceDao {
public:
	virtual void create(RoleInPlace &role) = 0;
	virtual bool fetch(RoleInPlace &role) = 0;
	virtual void fetchBy(std::vector<RoleInPlace> &roles,
			const Place &place) = 0;
	virtual bool hasUsersExcept(const Place &place,
			const User &user) = 0;
	virtual bool update(RoleInPlace &role) = 0;
	virtual bool remove(const RoleInPlace &role) = 0;

	virtual AccessLevel fetchAccessLevel(
			const Place &place,
			const User &user) = 0;

	/**
	 * Fetch all places associated with the given user.
	 * I.e. every role with an identity that belongs to
	 * the given user provides access to a place. Such
	 * places are returned in this call.
	 */
	virtual void fetchAccessiblePlaces(
			std::vector<Place> &list,
			const User &user,
			const AccessLevel &atLeast = AccessLevel::any()) = 0;
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

	void fetchAccessiblePlaces(
			std::vector<Place> &list,
			const User &user,
			const AccessLevel &atLeast = AccessLevel::any())
	{
		throw Poco::NotImplementedException(__func__);
	}

	void fetchBy(std::vector<RoleInPlace> &roles,
			const Place &place)
	{
		throw Poco::NotImplementedException(__func__);
	}

	bool hasUsersExcept(const Place &place, const User &user) {
		throw Poco::NotImplementedException(__func__);
	}

	static RoleInPlaceDao &instance();
};

}

#endif
