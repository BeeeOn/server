#ifndef BEEEON_DEFAULT_ACCESS_POLICY_H
#define BEEEON_DEFAULT_ACCESS_POLICY_H

#include "policy/GatewayAccessPolicy.h"
#include "policy/IdentityAccessPolicy.h"
#include "policy/LocationAccessPolicy.h"
#include "policy/DeviceAccessPolicy.h"
#include "policy/RoleAccessPolicy.h"
#include "policy/SensorHistoryAccessPolicy.h"
#include "dao/RoleInGatewayDao.h"
#include "dao/GatewayDao.h"
#include "dao/LocationDao.h"
#include "dao/DeviceDao.h"
#include "dao/UserDao.h"

namespace BeeeOn {

class DefaultAccessPolicy :
		public GatewayAccessPolicy,
		public IdentityAccessPolicy,
		public LocationAccessPolicy,
		public DeviceAccessPolicy,
		public RoleAccessPolicy,
		public SensorHistoryAccessPolicy {
public:
	DefaultAccessPolicy();

	void assure(
		const GatewayAccessPolicy::Action action,
		const PolicyContext &context,
		const Gateway &gateway) override;

	void assure(
		const IdentityAccessPolicy::Action action,
		const PolicyContext &context,
		const Identity &identity) override;
	void assure(
		const IdentityAccessPolicy::Action action,
		const PolicyContext &context,
		const VerifiedIdentity &identity) override;
	void assure(
		const IdentityAccessPolicy::Action action,
		const PolicyContext &context,
		const User &user) override;

	void assure(
		const LocationAccessPolicy::Action action,
		const PolicyContext &context,
		const Location &location) override;
	void assure(
		const LocationAccessPolicy::Action action,
		const PolicyContext &context,
		const Gateway &gateway) override;

	void assure(
		const DeviceAccessPolicy::Action action,
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway) override;
	void assure(
		const DeviceAccessPolicy::Action action,
		const PolicyContext &context,
		const Gateway &gateway) override;
	void assureMany(
		const DeviceAccessPolicy::Action action,
		const PolicyContext &context,
		const std::list<Device> &devices) override;

	void assure(
		const RoleAccessPolicy::Action action,
		const PolicyContext &context,
		const Gateway &gateway) override;
	void assure(
		const RoleAccessPolicy::Action action,
		const PolicyContext &context,
		const RoleInGateway &role) override;

	void assure(
		const SensorHistoryAccessPolicy::Action action,
		const PolicyContext &context,
		const Device &device) override;

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
	AccessLevel fetchAccessLevel(const PolicyContext &context, const Gateway &gateway);
	void assureAtLeast(
			const AccessLevel &current,
			const AccessLevel &required);

	/**
	 * Test whether the given role represents the PolicyContext.
	 */
	bool representsSelf(const RoleInGateway &role, const PolicyContext &self);
	bool representsSelf(const User &user, const PolicyContext &self);
	bool canSeeIdentity(const Identity &identity, const PolicyContext &self);
	bool canSeeIdentity(const VerifiedIdentity &identity, const PolicyContext &self);

	void doAssure(
		const DeviceAccessPolicy::Action action,
		const PolicyContext &context,
		const Gateway &gateway);

private:
	UserDao *m_userDao;
	GatewayDao *m_gatewayDao;
	LocationDao *m_locationDao;
	DeviceDao *m_deviceDao;
	RoleInGatewayDao *m_roleInGatewayDao;
};

}

#endif

