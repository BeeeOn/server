#ifndef BEEEON_SENSOR_SERVICE_H
#define BEEEON_SENSOR_SERVICE_H

#include <list>

#include <Poco/SharedPtr.h>

#include "model/Device.h"
#include "model/Sensor.h"
#include "service/Relation.h"

namespace BeeeOn {

/**
 * @brief Class SensorService is used for modules management and querying.
 */
class SensorService {
public:
	typedef Poco::SharedPtr<SensorService> Ptr;

	virtual ~SensorService();

	virtual bool fetch(Relation<Sensor, Device> &data) = 0;
	virtual void fetchMany(Relation<std::list<Sensor>, Device> &data) = 0;
};

}

#endif
