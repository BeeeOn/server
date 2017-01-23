#ifndef BEEEON_DEFAULT_ACCESS_POLICY_H
#define BEEEON_DEFAULT_ACCESS_POLICY_H

#include "di/InjectorTarget.h"
#include "policy/GatewayAccessPolicy.h"
#include "policy/LocationAccessPolicy.h"
#include "policy/DeviceAccessPolicy.h"
#include "dao/RoleInPlaceDao.h"
#include "dao/GatewayDao.h"
#include "dao/LocationDao.h"
#include "dao/DeviceDao.h"
#include "dao/UserDao.h"

namespace BeeeOn {

class DefaultAccessPolicy : public AbstractInjectorTarget,
		public GatewayAccessPolicy,
		public LocationAccessPolicy,
		public DeviceAccessPolicy {
public:
	DefaultAccessPolicy();

	void assureGet(const PolicyContext &context,
		const Gateway &gateway) override;
	void assureUnassign(const PolicyContext &context,
		const Gateway &gateway) override;
	void assureUpdate(const PolicyContext &context,
		const Gateway &gateway) override;
	void assureScanDevices(const PolicyContext &context,
		const Gateway &gateway) override;

	void assureGet(const PolicyContext &context,
		const Location &location);
	void assureCreateLocation(const PolicyContext &context,
		const Gateway &gateway);
	void assureUpdate(const PolicyContext &context,
		const Location &location);
	void assureRemove(const PolicyContext &context,
		const Location &location);

	void assureGet(const PolicyContext &context,
		const Device &device, const Gateway &gateway);
	void assureGetMany(const PolicyContext &context,
		const std::list<Device> &devices);
	void assureListActiveDevices(const PolicyContext &context,
		const Gateway &gateway);
	void assureListInactiveDevices(const PolicyContext &context,
		const Gateway &gateway);
	void assureUnregister(const PolicyContext &context,
		const Device &device, const Gateway &gateway);
	void assureActivate(const PolicyContext &context,
		const Device &device, const Gateway &gateway);
	void assureUpdate(const PolicyContext &context,
		const Device &device, const Gateway &gateway);

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

	void setDeviceDao(DeviceDao *dao)
	{
		m_deviceDao = dao;
	}

	void setRoleInPlaceDao(RoleInPlaceDao *dao)
	{
		m_roleInPlaceDao = dao;
	}

protected:
	AccessLevel fetchAccessLevel(const User &user, const Place &place);
	AccessLevel fetchAccessLevel(const User &user, const Gateway &gateway);
	void assureAtLeast(
			const AccessLevel &current,
			const AccessLevel &required);

private:
	UserDao *m_userDao;
	GatewayDao *m_gatewayDao;
	LocationDao *m_locationDao;
	DeviceDao *m_deviceDao;
	RoleInPlaceDao *m_roleInPlaceDao;
};

}

#endif

