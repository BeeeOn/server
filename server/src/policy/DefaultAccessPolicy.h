#ifndef BEEEON_DEFAULT_ACCESS_POLICY_H
#define BEEEON_DEFAULT_ACCESS_POLICY_H

#include "di/InjectorTarget.h"
#include "server/Session.h"
#include "dao/RoleInPlaceDao.h"
#include "dao/UserDao.h"

namespace BeeeOn {

class DefaultAccessPolicy : public AbstractInjectorTarget {
public:
	DefaultAccessPolicy();

	void setUserDao(UserDao *dao)
	{
		m_userDao = dao;
	}

	void setRoleInPlaceDao(RoleInPlaceDao *dao)
	{
		m_roleInPlaceDao = dao;
	}

protected:
	AccessLevel fetchAccessLevel(
			const ExpirableSession::Ptr session,
			const Place &place);
	void assureAtLeast(
			const AccessLevel &current,
			const AccessLevel &required);

private:
	UserDao *m_userDao;
	RoleInPlaceDao *m_roleInPlaceDao;
};

}

#endif

