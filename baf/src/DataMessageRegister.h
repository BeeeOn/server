/*
 * File:   DataMessageRegister.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef DATAMESSAGEREGISTER_H
#define DATAMESSAGEREGISTER_H

#include <map>
#include <mutex>
#include <set>
#include <vector>

#include "DataMessage.h"
#include "TaskInstance.h"

class DataMessageRegister {
public:
	/**
	 * Creates DataMessageRegister singleton instance.
	 * Must be called just once in entire program.
	 */
	static void createInstance();
	/**
	 * Returns pointer to DataMessageRegister singleton instance.
	 * @return Pointer to DataMessageRegister instance.
	 */
	static std::shared_ptr<DataMessageRegister> getInstance();
	/**
	 * Destructor of class DataMessageRegister.
	 */
	virtual ~DataMessageRegister();
	/**
	 * Activates all instances which should receive data
	 * from device which EUID is in passed DataMessage object.
	 * @param data_message Parsed DataMessage.
	 */
	void activateInstances(DataMessage data_message);
	/**
	 * Inserts new entry to DataMessageRegister.
	 * @param device_euid EUID of device from which should instance receive data.
	 * @param instance_ptr Pointer to instance which should receive messages from device.
	 */
	void insertEntry(long device_euid, TaskInstance* instance_ptr);
	/**
	 * Removes one entry from DataMessageRegister.
	 * @param device_euid EUID of device from which should not instance receive data anymore.
	 * @param instance_ptr Pointer to instance which deletes entry.
	 */
	void removeEntry(long device_euid, TaskInstance* instance_ptr);
	/**
	 * Removes entries of instance from DataMessageRegister.
	 * @param device_euids Set of EUIDs of devices.
	 * @param instance_ptr Pointer to instance which should not receive data from devices.
	 */
	void removeAllEntries(std::set<long /*device_euid*/> device_euids, TaskInstance* instance_ptr);

private:
	/**
	 * Constructor of class DataMessageRegister.
	 */
	DataMessageRegister();
	/**
	 * Delete copy constructor.
	 */
	DataMessageRegister(const DataMessageRegister& orig) = delete;
	/**
	 * Delete assignment operator.
	 */
	void operator=(const DataMessageRegister&) = delete;
	/**
	 * Singleton instance pointer.
	 */
	static std::shared_ptr<DataMessageRegister> m_instance;
	/**
	 * Mutex protecting m_message_register container from being
	 * accessed by more threads at the same time.
	 */
	std::mutex m_register_mx;
	/**
	 * Container storing which instance wants sensory data from which device.
	 */
	std::multimap<long /*device_euid*/, TaskInstance*/*instance*/> m_message_register;
	/**
	 * Finds and returns all instances which are registered to receive data from device.
	 * @param device_euid Searched EUID of device.
	 * @return Vector of instances which are registered to receive data from passed device.
	 */
	std::vector<TaskInstance*> returnAllRegisteredInstances(long device_euid);
};

#endif /* DATAMESSAGEREGISTER_H */
