#ifndef BEEEON_DEVICE_UNPAIR_WORK_H
#define BEEEON_DEVICE_UNPAIR_WORK_H

#include <Poco/SharedPtr.h>

#include "model/Device.h"
#include "work/Work.h"

namespace BeeeOn {

class DeviceUnpairWorkExecutor;

/**
 * Representation of a work that unpairs a device from its gateway.
 * The unpairing process usually requires some asynchronous communication
 * with other parts of the system.
 */
class DeviceUnpairWork : public Work {
public:
	typedef Poco::SharedPtr<DeviceUnpairWork> Ptr;

	DeviceUnpairWork();
	DeviceUnpairWork(const WorkID &id);

	/**
	 * Set when creating the work instance. The device must
	 * contain the owning Gateway.
	 */
	void setDevice(const Device &device);

	/**
	 * Device holding the Gateway we want to unpair from.
	 * We need Device::id() and Device::gateway().id() to be
	 * valid.
	 */
	const Device &device() const;

	/**
	 * Update the number of attempts done while trying to unpair a
	 * device. To be called from DeviceUnpairWorkExecutor or some
	 * persistance layer only.
	 */
	void setAttempt(unsigned int attempt);

	/**
	 * Return the number of this attempt. Initially, this is always 0.
	 * After N executions, the value is incremented automatically.
	 */
	unsigned int attempt() const;

private:
	unsigned int m_attempt;
	Device m_device;
};

}

#endif
