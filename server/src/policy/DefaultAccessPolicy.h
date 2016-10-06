#ifndef BEEEON_DEFAULT_ACCESS_POLICY_H
#define BEEEON_DEFAULT_ACCESS_POLICY_H

#include "di/InjectorTarget.h"
#include "server/Session.h"
#include "policy/PlaceAccessPolicy.h"
#include "policy/GatewayAccessPolicy.h"
#include "policy/LocationAccessPolicy.h"
#include "dao/RoleInPlaceDao.h"
#include "dao/GatewayDao.h"
#include "dao/LocationDao.h"
#include "dao/UserDao.h"

namespace BeeeOn {

class DefaultAccessPolicy : public AbstractInjectorTarget,
		public PlaceAccessPolicy,
		public GatewayAccessPolicy,
		public LocationAccessPolicy {
public:
	DefaultAccessPolicy();

	void assureGet(const ExpirableSession::Ptr session,
		const Place &place) override;
	void assureUpdate(const ExpirableSession::Ptr session,
		const Place &place) override;
	void assureRemove(const ExpirableSession::Ptr session,
		const Place &place) override;

	void assureGet(const ExpirableSession::Ptr session,
		const Gateway &gateway) override;
	void assureAssignGateway(const ExpirableSession::Ptr session,
		const Place &place) override;
	void assureUnassign(const ExpirableSession::Ptr session,
		const Gateway &gateway) override;
	void assureUpdate(const ExpirableSession::Ptr session,
		const Gateway &gateway) override;
	void assureScanDevices(const ExpirableSession::Ptr session,
		const Gateway &gateway) override;

	void assureGet(const ExpirableSession::Ptr session,
		const Location &location);
	void assureCreateLocation(const ExpirableSession::Ptr session,
		const Place &place);
	void assureUpdate(const ExpirableSession::Ptr session,
		const Location &location);
	void assureRemove(const ExpirableSession::Ptr session,
		const Location &location);

	void setUserDao(UserDao *dao)
	{
		m_userDao = dao;
	}

	void setGatewayDao(GatewayDao *dao)
	{
		m_gatewayDao = dao;
	}

	void setLocationDao(LocationDao *dao)
	{
		m_locationDao = dao;
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
	GatewayDao *m_gatewayDao;
	LocationDao *m_locationDao;
	RoleInPlaceDao *m_roleInPlaceDao;
};

}

#endif

