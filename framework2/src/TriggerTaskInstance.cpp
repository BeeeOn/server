/* 
 * File:   EventAlgorithmInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TriggerTaskInstance.h"

#include <iostream>

#include "DataMessageRegister.h"

TriggerTaskInstance::TriggerTaskInstance(unsigned int instance_id):
    TaskInstance(instance_id)
{
}

TriggerTaskInstance::~TriggerTaskInstance() {

}

void TriggerTaskInstance::activate(DataMessage data_message)
{
    std::lock_guard<std::mutex> lock(m_activation_mx);
    run(data_message);
}

void TriggerTaskInstance::registerDataMessage(unsigned long long device_euid) {
    std::cout << "registerDataMessage: " << device_euid << std::endl;
    DataMessageRegister::getInstance()->insertEntry(device_euid, this);
}
