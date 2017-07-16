#ifndef BEEEON_GWS_SENSOR_HISTORY_SERVICE_IMPL_H
#define BEEEON_GWS_SENSOR_HISTORY_SERVICE_IMPL_H

#include "dao/DeviceDao.h"
#include "dao/SensorHistoryDao.h"
#include "service/GWSSensorHistoryService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class GWSSensorHistoryServiceImpl :
	public GWSSensorHistoryService,
	public Transactional {
public:
	void setDeviceDao(DeviceDao::Ptr dao);
	void setSensorHistoryDao(SensorHistoryDao::Ptr dao);

	void insertMany(Device &device,
			const Poco::Timestamp &at,
			std::vector<ModuleValue> &values) override
	{
		return BEEEON_TRANSACTION(doInsertMany(device, at, values));
	}

	bool fetchLast(Device &device,
			ModuleInfo &module,
			Poco::Timestamp &at,
			double &value) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetchLast(device, module, at, value));
	}

protected:
	void doInsertMany(Device &device,
			const Poco::Timestamp &at,
			std::vector<ModuleValue> &values);

	bool doFetchLast(Device &device,
			ModuleInfo &module,
			Poco::Timestamp &at,
			double &value);

private:
	DeviceDao::Ptr m_deviceDao;
	SensorHistoryDao::Ptr m_sensorHistoryDao;
};

}

#endif
