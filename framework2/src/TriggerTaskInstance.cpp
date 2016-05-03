/* 
 * File:   EventAlgorithmInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TriggerTaskInstance.h"

#include <mutex>
#include <string>

#include "DataMessageRegister.h"
#include "Logger.h"

TriggerTaskInstance::TriggerTaskInstance(long instance_id, std::weak_ptr<TaskManager> owning_manager):
    TaskInstance(instance_id, owning_manager)
{
}

TriggerTaskInstance::~TriggerTaskInstance()
{
}

void TriggerTaskInstance::activate(DataMessage data_message)
{    
    std::lock_guard<std::mutex> lock(m_activation_mx);
    
    try {
        run(data_message);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("trigger_instance", "ERROR") << e.what() << std::endl;
    }
}

void TriggerTaskInstance::registerToReceiveDataFromDevice(long device_euid)
{
    try {
        logger.LOGFILE("trigger_instance", "INFO") << "Instance ID: " << m_instance_id
                                      << " | Register data message from device_euid: "
                                      << device_euid << std::endl;

        DataMessageRegister::getInstance()->insertEntry(device_euid, this);

        m_registered_device_euids.insert(device_euid);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("trigger_instance", "WARN") << e.what() << std::endl;
    }
}

void TriggerTaskInstance::removeEntryFromDataMessageRegiser(long device_euid)
{
    try {
        // Remove device_euid from DataMessageRegister.
        DataMessageRegister::getInstance()->removeEntryOfInstance(device_euid, this);
        // Remove device euid of registered device from internal container.
        m_registered_device_euids.erase(device_euid);

    }
    catch (const std::exception& e) {
        logger.LOGFILE("trigger_instance", "WARN") << e.what() << std::endl;
    }
}

void TriggerTaskInstance::deleteFromControlComponent()
{
    std::lock_guard<std::mutex> lock(m_activation_mx);
    try {
        // Remove all entries of this instance from DataMessageRegister.
        DataMessageRegister::getInstance()->removeAllEntriesOfInstance(m_registered_device_euids, this);
        // Remove all device euids of registered devices from internal container.
        m_registered_device_euids.clear();

    }
    catch (const std::exception& e) {
        logger.LOGFILE("trigger_instance", "WARN") << e.what() << std::endl;
    }
}
