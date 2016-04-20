/* 
 * File:   DataMessageRegister.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "DataMessageRegister.h"

#include <stdexcept>

#include "DataMessage.h"
#include "Logger.h"
#include "TaskInstance.h"

// Definition of singleton instance.
std::shared_ptr<DataMessageRegister> DataMessageRegister::m_instance;

DataMessageRegister::DataMessageRegister()
{
}

DataMessageRegister::~DataMessageRegister()
{
}

void DataMessageRegister::createInstance()
{
    if (!m_instance) {
        logger.LOGFILE("data_message_register", "INFO") << "DatabaseInterface created." << std::endl;
        m_instance = std::shared_ptr<DataMessageRegister>(new DataMessageRegister);
    }
}

std::shared_ptr<DataMessageRegister> DataMessageRegister::getInstance()
{
    if (m_instance) {
        return m_instance;
    }
    else {
        throw std::runtime_error("DataMessageRegister singleton was not created or already destructed.");
    }
}

void DataMessageRegister::activateInstances(DataMessage data_message)
{
        std::vector<TaskInstance*> to_activate = returnAllRegisteredInstances(data_message.device_euid);
        
        for (TaskInstance* task_ptr : to_activate) {
            
            task_ptr->activate(data_message);
        }
}

void DataMessageRegister::insertEntry(long device_euid, TaskInstance* task_instance)
{ 
    std::lock_guard<std::mutex> lock(m_register_mx);

    m_message_register.emplace(device_euid, task_instance);
    
}
void DataMessageRegister::removeEntryOfInstance(long device_euid, TaskInstance* instance_ptr)
{
    std::lock_guard<std::mutex> lock(m_register_mx);
    
    auto range_of_euids =  m_message_register.equal_range(device_euid);
    for (auto it = range_of_euids.first; it != range_of_euids.second; it++) {
            
            if (it->second == instance_ptr) {
                m_message_register.erase(it);
            }
    }
}

void DataMessageRegister::removeAllEntriesOfInstance(std::set<long> device_euids, TaskInstance* instance_ptr)
{
    std::lock_guard<std::mutex> lock(m_register_mx);
    
    for (auto device_euid : device_euids) {
        
        auto range_of_euids =  m_message_register.equal_range(device_euid);

        for (auto it = range_of_euids.first; it != range_of_euids.second; it++) {
            
            if (it->second == instance_ptr) {
                m_message_register.erase(it);
            }
        }
    }
}

std::vector<TaskInstance*> DataMessageRegister::returnAllRegisteredInstances(long device_euid)
{
    std::lock_guard<std::mutex> lock(m_register_mx);
    
    std::vector<TaskInstance*> to_return;
    
    // Find iterator to where device_euid appears first, and iterator where it appears last in m_message_register.
    // Stored in std::pair.
    auto range_of_euids = m_message_register.equal_range(device_euid);
    for (auto it = range_of_euids.first; it != range_of_euids.second; it++) {

        to_return.push_back(it->second);
    }
    return to_return;
}
