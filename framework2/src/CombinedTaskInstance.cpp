/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CombinedInstance.cpp
 * Author: mrmaidx
 * 
 * Created on 21. dubna 2016, 22:34
 */

#include "CombinedTaskInstance.h"

#include <stdexcept>

#include "Calendar.h"
#include "DataMessageRegister.h"

#include "Logger.h"

CombinedTaskInstance::CombinedTaskInstance(int instance_id, std::weak_ptr<TaskManager> owning_manager):
    TaskInstance(instance_id, owning_manager),
    TimedTaskInstance(instance_id, owning_manager),
    TriggerTaskInstance(instance_id, owning_manager)
{
}

CombinedTaskInstance::~CombinedTaskInstance() {
}

void CombinedTaskInstance::deleteFromControlComponent()
{
    try {
        // Remove all activations of instance from Calendar.
        Calendar::getInstance()->removeAllActivationsOfInstance(m_activation_times, this);
        // Remove all information about activations from internal container.
        m_activation_times.clear();
        // Remove all entries of this instance from DataMessageRegister.
        DataMessageRegister::getInstance()->removeAllEntriesOfInstance(m_registered_device_euids, this);
        // Remove all device euids of registered devices from internal container.
        m_registered_device_euids.clear();
    }
    catch (const std::exception& e) {
        logger.LOGFILE("combined_instance", "WARN") << e.what() << std::endl;
    }
}
