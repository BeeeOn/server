#ifndef BEEEON_MOCK_INIT_H
#define BEEEON_MOCK_INIT_H

#include <vector>

#include "dao/UserDao.h"
#include "dao/GatewayDao.h"
#include "dao/LocationDao.h"
#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "dao/SensorHistoryDao.h"
#include "dao/Transactional.h"
#include "model/Location.h"
#include "model/DeviceInfo.h"
#include "provider/InfoProvider.h"

namespace BeeeOn {

class DeviceDao;

class UIMockInit : public Transactional {
public:
	UIMockInit()
	{
	}

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

	void setIdentityDao(IdentityDao *dao)
	{
		m_identityDao = dao;
	}

	void setVerifiedIdentityDao(VerifiedIdentityDao *dao)
	{
		m_verifiedIdentityDao = dao;
	}

	void setDeviceInfoProvider(InfoProvider<DeviceInfo> *provider)
	{
		m_infoProvider = provider;
	}

	void setSensorHistoryDao(SensorHistoryDao *dao)
	{
		m_sensorHistoryDao = dao;
	}

	void initAll();

protected:
	void initUsers();
	void initGateways();
	void initLocations(std::vector<Location> &locations);
	void initDevices(const std::vector<Location> &locations);
	void initSensorData();

private:
	UserDao *m_userDao;
	GatewayDao *m_gatewayDao;
	LocationDao *m_locationDao;
	DeviceDao *m_deviceDao;
	IdentityDao *m_identityDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
	InfoProvider<DeviceInfo> *m_infoProvider;
	SensorHistoryDao *m_sensorHistoryDao;
};

}

#endif
