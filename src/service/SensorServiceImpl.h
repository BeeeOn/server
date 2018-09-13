#pragma once

#include "dao/DeviceDao.h"
#include "dao/SensorHistoryDao.h"
#include "policy/SensorAccessPolicy.h"
#include "provider/DeviceInfoProvider.h"
#include "service/SensorService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class SensorServiceImpl : public SensorService, public Transactional {
public:
	void setDeviceDao(DeviceDao::Ptr deviceDao);
	void setSensorHistoryDao(SensorHistoryDao::Ptr sensorHistoryDao);
	void setAccessPolicy(SensorAccessPolicy::Ptr policy);

	bool fetch(Relation<Sensor, Device> &data) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(data));
	}

	void fetchMany(Relation<std::list<Sensor>, Device> &data) override
	{
		BEEEON_TRANSACTION(doFetchMany(data));
	}

protected:
	bool doFetch(Relation<Sensor, Device> &data);
	void doFetchMany(Relation<std::list<Sensor>, Device> &data);

private:
	DeviceDao::Ptr m_deviceDao;
	SensorHistoryDao::Ptr m_sensorHistoryDao;
	SensorAccessPolicy::Ptr m_policy;
};

}
