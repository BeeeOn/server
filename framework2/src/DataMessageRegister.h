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
     * Creates singleton instance. Must be called just once in entire program.
     */
    static void createInstance();
    /**
     * Serves to access pointer to singleton object of this class.
     * @return Shared pointer to singleton instance.
     */
    static std::shared_ptr<DataMessageRegister> getInstance();
    /**
     * Virtual destructor of class DataMessageRegister.
     */
    virtual ~DataMessageRegister();
    /**
     * Activates all instances which registered to receive data from device
     * which EUID is in passed DataMessage object.
     * @param data_message DataMessage received by GatewayServer.
     */
    void activateInstances(DataMessage data_message);
    /**
     * Inserts new entry to m_message_register.
     * @param device_euid EUID of registered device.
     * @param instance_ptr Pointer to instance which should receive messages from device.
     */
    void insertEntry(long device_euid, TaskInstance* instance_ptr);
    /**
     * Removes one entry from m_message_register.
     * @param device_euid EUID of registered device.
     * @param instance_ptr Pointer to instance which deletes entry.
     */
    void removeEntry(long device_euid, TaskInstance* instance_ptr);
    
    /**
     * Removes all entries of instance from m_message_register.
     * @param device_euids Set of all EUIDs of registered devices.
     * @param instance_ptr Pointer to instance which deletes entries.
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
     * Container storing which instance wants data message from which device.
     */
    std::multimap<long /*device_euid*/, TaskInstance*/*instance*/> m_message_register; 
    /**
     * Finds and returns all instances which are registered to receive data from device_euid.
     * @param device_euid Searched EUID of device.
     * @return Vector of instances which are registered to receive data.
     */
    std::vector<TaskInstance*> returnAllRegisteredInstances(long device_euid); 
};

#endif /* DATAMESSAGEREGISTER_H */
