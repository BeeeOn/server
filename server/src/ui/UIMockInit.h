#ifndef BEEEON_MOCK_INIT_H
#define BEEEON_MOCK_INIT_H

#include "di/InjectorTarget.h"
#include "dao/UserDao.h"
#include "dao/GatewayDao.h"
#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"

namespace BeeeOn {

class DeviceDao;

class UIMockInit : public AbstractInjectorTarget {
public:
	UIMockInit()
	{
		injector<UIMockInit, UserDao>("userDao",
			&UIMockInit::setUserDao);
		injector<UIMockInit, GatewayDao>("gatewayDao",
			&UIMockInit::setGatewayDao);
		injector<UIMockInit, DeviceDao>("deviceDao",
			&UIMockInit::setDeviceDao);
		injector<UIMockInit, IdentityDao>("identityDao",
			&UIMockInit::setIdentityDao);
		injector<UIMockInit, VerifiedIdentityDao>("verifiedIdentityDao",
			&UIMockInit::setVerifiedIdentityDao);
	}

	void setUserDao(UserDao *dao)
	{
		m_userDao = dao;
	}

	void setGatewayDao(GatewayDao *dao)
	{
		m_gatewayDao = dao;
	}

	void setDeviceDao(DeviceDao *dao)
	{
		m_deviceDao = dao;
	}

	void setIdentityDao(IdentityDao *dao)
	{
		m_identityDao = dao;
	}

	void setVerifiedIdentityDao(VerifiedIdentityDao *dao)
	{
		m_verifiedIdentityDao = dao;
	}

protected:
	void initUsers();
	void initGateways();
	void initDevices();
	void injectionDone();

private:
	UserDao *m_userDao;
	GatewayDao *m_gatewayDao;
	DeviceDao *m_deviceDao;
	IdentityDao *m_identityDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
};

}

#endif
