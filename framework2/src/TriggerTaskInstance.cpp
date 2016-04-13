/* 
 * File:   EventAlgorithmInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TriggerTaskInstance.h"

#include <iostream>

#include "DataMessageRegister.h"

TriggerTaskInstance::TriggerTaskInstance(unsigned int instance_id, TaskManager *owning_manager):
    TaskInstance(instance_id, owning_manager)
{
}

TriggerTaskInstance::~TriggerTaskInstance()
{
    removeFromDataMessageRegister();
}

void TriggerTaskInstance::activate(DataMessage data_message)
{
    std::lock_guard<std::mutex> lock(m_activation_mx);
    run(data_message);
}

void TriggerTaskInstance::registerDataMessage(long device_euid)
{
    std::cout << "registerDataMessage: " << device_euid << std::endl;
    DataMessageRegister::getInstance()->insertEntry(device_euid, this);
    m_registered_device_euids.insert(device_euid);
}

void TriggerTaskInstance::removeFromDataMessageRegister()
{
    DataMessageRegister::getInstance()->removeAllEntriesOfInstance(m_registered_device_euids, this);
}
