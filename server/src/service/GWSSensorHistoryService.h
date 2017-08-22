#ifndef SERVER_GWS_SENSOR_HISTORY_SERVICE_H
#define SERVER_GWS_SENSOR_HISTORY_SERVICE_H

#include <vector>

#include <Poco/SharedPtr.h>
#include <Poco/Timestamp.h>

#include "model/Device.h"
#include "model/ModuleInfo.h"
#include "model/ModuleValue.h"

namespace BeeeOn {

class GWSSensorHistoryService {
public:
	typedef Poco::SharedPtr<GWSSensorHistoryService> Ptr;

	virtual ~GWSSensorHistoryService();

	/**
	 * Store vector of measured values with the given timestamp,
	 * from the given device.
	 *
	 * Last seen time of the device is also updated.
	 *
	 * @throw NotFoundException when the device does not exists.
	 * @throw IllegalStateException when updating the device fails.
	 * @throw InvalidArgumentException when the device does not contains
	 * such module.
	 * @throw IllegalStateException when the data insertion fails.
	 */
	virtual void insertMany(Device &device,
		const Poco::Timestamp &at,
		std::vector<ModuleValue> &values) = 0;

	/**
	 * Retrieve the last measured value and its timestamp,
	 * from the given module of the given device.
	 *
	 * @throw NotFoundException when the device does not exists.
	 * @throw NotFoundException when the device does not contains such module.
	 *
	 * @return false if retrieve of the last measured value fails.
	 */
	virtual bool fetchLast(Device &device,
		ModuleInfo &module,
		Poco::Timestamp &at,
		double &value) = 0;
};

}

#endif
