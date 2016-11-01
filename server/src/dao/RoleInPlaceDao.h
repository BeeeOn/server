#ifndef BEEEON_ROLE_IN_PLACE_DAO_H
#define BEEEON_ROLE_IN_PLACE_DAO_H

#include <vector>

#include "di/InjectorTarget.h"
#include "model/RoleInPlace.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "dao/PlaceDao.h"

namespace BeeeOn {

class User;

class RoleInPlaceDao {
public:
	virtual void create(RoleInPlace &role) = 0;
	virtual bool fetch(RoleInPlace &role) = 0;
	virtual void fetchBy(std::vector<RoleInPlace> &roles,
			const Place &place) = 0;
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

	static RoleInPlaceDao &instance();
};

class MockRoleInPlaceDao : public AbstractInjectorTarget,
		public MockDao<RoleInPlace, RoleInPlaceDao> {
public:
	MockRoleInPlaceDao();

	AccessLevel fetchAccessLevel(
			const Place &place,
			const User &user);

	void fetchAccessiblePlaces(
			std::vector<Place> &list,
			const User &user,
			const AccessLevel &atLeast = AccessLevel::any());

	void fetchBy(std::vector<RoleInPlace> &roles,
			const Place &place);

	void setPlaceDao(PlaceDao *dao)
	{
		m_placeDao = dao;
	}

protected:
	RoleInPlaceID nextID()
	{
		return RoleInPlaceID::random();
	}

	PlaceDao *m_placeDao;
};

}

#endif
