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
#include <vector>

#include "TaskInstance.h"

class DataMessageRegister {
public:
    DataMessageRegister();
    
    DataMessageRegister(const DataMessageRegister& orig) = delete;
    
    virtual ~DataMessageRegister();

    static std::shared_ptr<DataMessageRegister> getInstance();

    void insertEntry(unsigned long long device_euid, TaskInstance* task_instance);
   
    std::vector<TaskInstance*> returnAllEntries(unsigned long long device_euid); 
    
private:
    
    std::mutex m_register_mutex;
    /**
     * Singleton instance pointer.
     */
    static std::shared_ptr<DataMessageRegister> m_instance;
    
    std::map<unsigned long long /*device_euid*/, std::vector<TaskInstance*> /*instances*/> m_msg_register;
    
};

#endif /* DATAMESSAGEREGISTER_H */

