/* 
 * File:   DataMessageRegister.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "DataMessageRegister.h"

#include <iostream>

#include "DataMessage.h"

std::shared_ptr<DataMessageRegister> DataMessageRegister::m_instance;

DataMessageRegister::DataMessageRegister() {
}


DataMessageRegister::~DataMessageRegister() {
}

std::shared_ptr<DataMessageRegister> DataMessageRegister::getInstance()
{
    //std::cout << "DatabaseInterface::getInstance()" << std::endl;
    
    if (!m_instance) {
        m_instance = std::make_shared<DataMessageRegister>();
        }
    else {
        return m_instance;
    }
}

void DataMessageRegister::insertEntry(long device_euid, TaskInstance* task_instance)
{
    
    std::lock_guard<std::mutex> lock(m_register_mutex);

    m_message_register.emplace(device_euid, task_instance);
    
}

std::vector<TaskInstance*> DataMessageRegister::returnAllEntries(long device_euid)
{
    std::lock_guard<std::mutex> lock(m_register_mutex);
    
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

void DataMessageRegister::removeAllEntriesOfInstance(std::set<long> registered_device_euids, TaskInstance* instance_ptr)
{
    std::lock_guard<std::mutex> lock(m_register_mutex);
    
    for (auto registered_device_euid : registered_device_euids) {
        
        auto range_of_euids =  m_message_register.equal_range(registered_device_euid);

        for (auto it = range_of_euids.first; it != range_of_euids.second; it++) {
            
            if (it->second == instance_ptr) {
                m_message_register.erase(it);
            }
        }
    }
}
