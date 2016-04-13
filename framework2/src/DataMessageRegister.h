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

#include "TaskInstance.h"

class DataMessageRegister {
public:
    DataMessageRegister();
    
    DataMessageRegister(const DataMessageRegister& orig) = delete;
    
    virtual ~DataMessageRegister();

    static std::shared_ptr<DataMessageRegister> getInstance();

    void insertEntry(long device_euid, TaskInstance* instance_ptr);
   
    void removeAllEntriesOfInstance(std::set<long /*device_euid*/> registered_device_euids, TaskInstance* instance_ptr);
    
    std::vector<TaskInstance*> returnAllEntries(long device_euid); 
    
private:
    
    std::mutex m_register_mutex;
    /**
     * Singleton instance pointer.
     */
    static std::shared_ptr<DataMessageRegister> m_instance;
    
    std::multimap<long /*device_euid*/, TaskInstance*/*instance*/> m_message_register; 
};

#endif /* DATAMESSAGEREGISTER_H */
