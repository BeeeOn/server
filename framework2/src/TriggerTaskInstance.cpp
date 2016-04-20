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

TriggerTaskInstance::TriggerTaskInstance(int instance_id, std::weak_ptr<TaskManager> owning_manager):
    TaskInstance(instance_id, owning_manager)
{
}

TriggerTaskInstance::~TriggerTaskInstance()
{
    // Before destruction remove instance from DataMessageRegister.
    removeAllFromDataMessageRegister();
}

void TriggerTaskInstance::activate(DataMessage data_message)
{
    logger.LOGFILE("trigger_instance", "TRACE") << "TriggerTaskInstance::activate - enter" << std::endl;
    
    std::lock_guard<std::mutex> lock(m_instance_mx);
    
    try {
        run(data_message);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("trigger_instance", "ERROR") << e.what() << std::endl;
    }
    
    logger.LOGFILE("trigger_instance", "TRACE") << "TriggerTaskInstance::activate - leave" << std::endl;
}

void TriggerTaskInstance::registerToReceiveDataFromDevice(long device_euid)
{
    logger.LOGFILE("trigger_instance", "TRACE") << "TriggerTaskInstance::registerToReceiveDataFromDevice - enter" << std::endl;
    
    try {
        logger.LOGFILE("trigger_instance", "INFO") << "Instance ID: " << m_instance_id
                                      << " | Register data message from device_euid: "
                                      << device_euid << std::endl;

        DataMessageRegister::getInstance()->insertEntry(device_euid, this);

        m_registered_device_euids.insert(device_euid);

        debugPrintRegisteredDeviceEuids();
    }
    catch (const std::exception& e) {
        logger.LOGFILE("trigger_instance", "WARN") << e.what() << std::endl;
    }
    logger.LOGFILE("trigger_instance", "TRACE") << "TriggerTaskInstance::registerToReceiveDataFromDevice - leave" << std::endl;
}

void TriggerTaskInstance::removeEntryFromDataMessageRegiser(long device_euid)
{
    logger.LOGFILE("trigger_instance", "TRACE") << "TriggerTaskInstance::removeEntryFromDataMessageRegiser - enter" << std::endl;
    
    try {
        // Remove device_euid from DataMessageRegister.
        DataMessageRegister::getInstance()->removeEntryOfInstance(device_euid, this);
        // Remove device euid of registered device from internal container.
        m_registered_device_euids.erase(device_euid);

        debugPrintRegisteredDeviceEuids();
    }
    catch (const std::exception& e) {
        logger.LOGFILE("trigger_instance", "WARN") << e.what() << std::endl;
    }
    logger.LOGFILE("trigger_instance", "TRACE") << "TriggerTaskInstance::removeEntryFromDataMessageRegiser - leave" << std::endl;
}

void TriggerTaskInstance::removeAllFromDataMessageRegister()
{
    logger.LOGFILE("trigger_instance", "TRACE") << "TriggerTaskInstance::removeAllFromDataMessageRegister - enter" << std::endl;
    
    try {
        // Remove all entries of this instance from DataMessageRegister.
        DataMessageRegister::getInstance()->removeAllEntriesOfInstance(m_registered_device_euids, this);
        // Remove all device euids of registered devices from internal container.
        m_registered_device_euids.clear();

        debugPrintRegisteredDeviceEuids();
    }
    catch (const std::exception& e) {
        logger.LOGFILE("trigger_instance", "WARN") << e.what() << std::endl;
    }
    logger.LOGFILE("trigger_instance", "TRACE") << "TriggerTaskInstance::removeAllFromDataMessageRegister - leave" << std::endl;
}

void TriggerTaskInstance::debugPrintRegisteredDeviceEuids()
{
    std::string registered_devices;
    if (m_registered_device_euids.size() == 0) {
        registered_devices += "none";
    }
    else {
        for (auto device_euid : m_registered_device_euids) {
        registered_devices += std::to_string(device_euid);
        registered_devices += ", ";
        }
    }
    logger.LOGFILE("trigger_instance", "DEBUG") << "Instance ID: " << m_instance_id
                                   << " | Registered devices: "
                                   << registered_devices << std::endl;
}
