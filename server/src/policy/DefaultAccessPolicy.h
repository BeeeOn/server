#ifndef BEEEON_DEFAULT_ACCESS_POLICY_H
#define BEEEON_DEFAULT_ACCESS_POLICY_H

#include "di/InjectorTarget.h"
#include "policy/GatewayAccessPolicy.h"
#include "policy/LocationAccessPolicy.h"
#include "policy/DeviceAccessPolicy.h"
#include "policy/RoleAccessPolicy.h"
#include "dao/RoleInGatewayDao.h"
#include "dao/GatewayDao.h"
#include "dao/LocationDao.h"
#include "dao/DeviceDao.h"
#include "dao/UserDao.h"

namespace BeeeOn {

class DefaultAccessPolicy : public AbstractInjectorTarget,
		public GatewayAccessPolicy,
		public LocationAccessPolicy,
		public DeviceAccessPolicy,
		public RoleAccessPolicy {
public:
	DefaultAccessPolicy();

	void assureRegister(const PolicyContext &context,
		const Gateway &gateway) override;
	void assureGet(const PolicyContext &context,
		const Gateway &gateway) override;
	void assureUnregister(const PolicyContext &context,
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

	void assureInvite(
		const PolicyContext &context,
		const Gateway &gateway,
		const AccessLevel &as) override;
	void assureList(
		const PolicyContext &context,
		const Gateway &gateway) override;
	void assureRemove(
		const PolicyContext &context,
		const RoleInGateway &role) override;
	void assureUpdate(
		const PolicyContext &context,
		const RoleInGateway &role) override;

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

	void setRoleInGatewayDao(RoleInGatewayDao *dao)
	{
		m_roleInGatewayDao = dao;
	}

protected:
	AccessLevel fetchAccessLevel(const User &user, const Gateway &gateway);
	void assureAtLeast(
			const AccessLevel &current,
			const AccessLevel &required);

private:
	UserDao *m_userDao;
	GatewayDao *m_gatewayDao;
	LocationDao *m_locationDao;
	DeviceDao *m_deviceDao;
	RoleInGatewayDao *m_roleInGatewayDao;
};

}

#endif

