/* 
 * File:   DataMessageRegister.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef DATAMESSAGEREGISTER_H
#define DATAMESSAGEREGISTER_H

#include <map> // std::multimap
#include <mutex> // std::mutex, std::lock_guard
#include <set> // std::set
#include <vector> // std::vector

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
     * Virtual destructor.
     */
    virtual ~DataMessageRegister();
    /**
     * Activates all instances which registered to receive data from device
     * which EUID is in passed DataMessage object.
     * @param data_message DataMessage received by GatewayServer.
     */
    void activateInstances(DataMessage data_message);
    
    void insertEntry(long device_euid, TaskInstance* instance_ptr);
   
    void removeEntryOfInstance(long device_euid, TaskInstance* instance_ptr);
    
    void removeAllEntriesOfInstance(std::set<long /*device_euid*/> device_euids, TaskInstance* instance_ptr);

private:
    /**
     * Have constructor as private.
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
    
    /** Singleton instance pointer. */
    static std::shared_ptr<DataMessageRegister> m_instance;
    
    /** Mutex protecting m_message_register container from being
     *  accessed by more threads at the same time.
     */
    std::mutex m_register_mx;

    /** Container storing which instance wants data message from which device. */
    std::multimap<long /*device_euid*/, TaskInstance*/*instance*/> m_message_register; 
    
    /**
     * Finds and returns all instances which are registered to receive data from device_euid.
     * @param device_euid Searched EUID of device.
     * @return Vector of instances which are registered to receive data.
     */
    std::vector<TaskInstance*> returnAllRegisteredInstances(long device_euid); 
};

#endif /* DATAMESSAGEREGISTER_H */
