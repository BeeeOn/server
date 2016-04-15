/* 
 * File:   DataMessageRegister.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "DataMessageRegister.h"

#include <iostream>
#include <stdexcept>


#include "DataMessage.h"
#include "Logger.h"
#include "TaskInstance.h"

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
        // Not using std::make_shared because it requires public constructor.
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
        std::cout << "to activate: " << to_activate.size() << std::endl;
        
        for (TaskInstance* task_ptr : to_activate) {
            
            task_ptr->activate(data_message);
        }
}

void DataMessageRegister::insertEntry(long device_euid, TaskInstance* task_instance)
{
    logger.LOGFILE("core", "TRACE") << "DataMessageRegister::insertEntry - enter" << std::endl;
    std::lock_guard<std::mutex> lock(m_register_mx);

    m_message_register.emplace(device_euid, task_instance);
    logger.LOGFILE("core", "TRACE") << "DataMessageRegister::insertEntry - leave" << std::endl;
    
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
    
    std::cout << "returnAllEntries device_euid: " << device_euid << std::endl;
    std::vector<TaskInstance*> to_return;
    
    // Find iterator to where device_euid appears first, and iterator where it appears last in m_message_register.
    // Stored in std::pair.
    auto range_of_euids = m_message_register.equal_range(device_euid);
    for (auto it = range_of_euids.first; it != range_of_euids.second; it++) {

        to_return.push_back(it->second);
    }

    std::cout << "to return: " << to_return.size() << std::endl;
    return to_return;
}
