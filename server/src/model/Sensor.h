#ifndef BEEEON_SENSOR_H
#define BEEEON_SENSOR_H

#include <string>

#include <Poco/Nullable.h>
#include <Poco/Timestamp.h>

#include "model/ModuleInfo.h"
#include "model/SimpleID.h"

namespace BeeeOn {

/**
 * @brief Class Sensor represents a one of many modules provided by a Device.
 *
 * Each Sensor has associated some meta information (data type, some characteristics, ...).
 * For each module, we are usually interested in its value at a certain point in time.
 * Usually, we want to see the last measured value.
 *
 * The measured value can have three possible states:
 *
 * * never measured (does not have any timestamp)
 * * invalid (e.g. sensor is physically deactivated)
 * * valid
 *
 * Thus, it is necessary to handle two invalid states. The <em>never measured</em> state is determined
 * from the timestamp. If there is no timestamp (null) there is no value available. The second invalid state
 * is determined from the value itself. If there is a valid timestamp but the value is invalid (NAN),
 * we know that there was an attempt to access such sensor by the system but the sensor was unable to give
 * any reasonable value.
 */
class Sensor : public Entity<SimpleID> {
public:
	Sensor();
	Sensor(const ID &id);

	void setInfo(const ModuleInfo &info);
	const ModuleInfo &info() const;

	void setName(const std::string &name);

	/**
	 * @return custom name given by user.
	 */
	std::string name() const;

	void setValue(const double value);

	/**
	 * @return value measured by the referenced module at the time given by method at().
	 * @see Sensor::at()
	 */
	double value() const;

	void setAt(const Poco::Nullable<Poco::Timestamp> &at);

	/**
	 * @return time when the value has been measured. If the at() returns NULL,
	 * there is no known value (at least for a given time period).
	 */
	Poco::Nullable<Poco::Timestamp> at() const;

private:
	ModuleInfo m_info;
	std::string m_name;
	double m_value;
	Poco::Nullable<Poco::Timestamp> m_at;
};

typedef Sensor::ID SensorID;

}

#endif
