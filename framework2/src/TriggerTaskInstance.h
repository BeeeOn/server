/* 
 * File:   TriggerTaskInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 22. January 2016
 */

#ifndef TRIGGERTASKINSTANCE_H
#define TRIGGERTASKINSTANCE_H

#include "DataMessage.h"
#include "TaskInstance.h"

#include <mutex>
#include <set>

class TriggerTaskInstance: public TaskInstance 
{
public:
    
    
    TriggerTaskInstance(int instance_id, TaskManager *owning_manager);
    
    /**
     * Virtual destructor of class TriggerTaskInstance. Removes all entries
     * with this instance from DataMessageRegister before instance is deleted.
     */
    virtual ~TriggerTaskInstance();

    /**
     * Activates instance. Protects run function with activation mutex,
     * so it's not possible to run one instance more times at the same time.
     * @param data_message Message with data from registered device_euid.
     */
    void activate(DataMessage data_message) override;
    
    /**
     * Virtual function which is called every time gateway sends a message
     * to server with data from device this instance registered to DataMessageRegister.
     * @param data_message Message with data from registered device_euid.
     */
    virtual void run(DataMessage data_message) = 0;

    /**
     * 
     * @param device_euid
     */
    void registerToReceiveDataFromDevice(long device_euid);
    
    
    void removeAllFromDataMessageRegister();
    
    
    void removeEntryFromDataMessageRegiser(long device_euid);
    
private:

    std::set<long /*device_euid*/> m_registered_device_euids;
    
    std::mutex m_instance_mx;
    
    void debugPrintRegisteredDeviceEuids();
};

#endif /* TRIGGERTASKINSTANCE_H */

