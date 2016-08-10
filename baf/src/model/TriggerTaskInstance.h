/*
 * File:   TriggerTaskInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 22. January 2016
 */

#ifndef TRIGGERTASKINSTANCE_H
#define TRIGGERTASKINSTANCE_H

#include "model/DataMessage.h"
#include "model/TaskInstance.h"

#include <mutex>
#include <set>

class TriggerTaskInstance: public virtual TaskInstance
{
public:
	/**
	 * Constructor of class TriggerTaskInstance.
	 * @param instance_id Identificator of created instance.
	 * @param owning_manager Pointer to manager of this instance.
	 */
	TriggerTaskInstance(long instance_id, std::weak_ptr<TaskManager> owning_manager);
	/**
	 * Destructor of class TriggerTaskInstance.
	 */
	virtual ~TriggerTaskInstance();
	/**
	 * Activates instance. Protects run function with activation mutex,
	 * so it's not possible to run one instance more times at the same time.
	 * @param data_message Object with sensory data from device.
	 */
	void activate(DataMessage data_message) override;
	/**
	 * Virtual function which is called every time gateway sends a message
	 * to server with data from device this instance registered to DataMessageRegister.
	 * @param data_message Message with data from registered device_euid.
	 */
	virtual void run(DataMessage data_message) = 0;
	/**
	 * Register instance to receive data messages from device.
	 * @param device_euid EUID of device from which to receive messages.
	 */
	void registerToReceiveDataFromDevice(long device_euid);
	/**
	 * Removes one entry from DataMessageRegister.
	 * After this instance won't receive any data messages from device.
	 * @param device_euid EUID of device from which not to receive messages anymore.
	 */
	void removeEntryFromDataMessageRegiser(long device_euid);
	/**
	 * Removes all entries from DataMessageRegister of instance.
	 * After this instance won't receive any data messages.
	 */
	virtual void deleteFromControlComponent() override;

protected:
	/**
	 * Set of all devices from which instance receives data messages.
	 */
	std::set<long /*device_euid*/> m_registered_device_euids;
};

#endif /* TRIGGERTASKINSTANCE_H */

