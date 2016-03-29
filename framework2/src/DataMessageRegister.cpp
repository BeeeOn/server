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

void DataMessageRegister::insertEntry(unsigned long long device_euid, TaskInstance* task_instance)
{
    std::lock_guard<std::mutex> lock(m_register_mutex);

    auto found = m_msg_register.find(device_euid);
    if (found == m_msg_register.end()) {
        // Device is not yet in registry.
        std::cout << "Device not yet in registry." << std::endl;
        m_msg_register.insert({device_euid, std::vector<TaskInstance*>({task_instance})});
    
    }
    else {
        // Device already is in registry. Just push instance pointer.
        found->second.push_back(task_instance);
    }

    for (auto entry : m_msg_register) {
        std::cout << "REGISTER ENTRY: " << entry.first << " vector size: " << entry.second.size() << std::endl;
        
    }
}

std::vector<TaskInstance*> DataMessageRegister::returnAllEntries(unsigned long long device_euid)
{
    std::lock_guard<std::mutex> lock(m_register_mutex);
    
    std::cout << "returnAllEntries device_euid: " << device_euid << std::endl;
    std::vector<TaskInstance*> to_return;
    
    auto found = m_msg_register.find(device_euid);
    if (found != m_msg_register.end()) {
        std::cout << "found entry" << std::endl;
        to_return = found->second;
    }
    
    std::cout << "to return: " << to_return.size() << std::endl;
    return to_return;
}
