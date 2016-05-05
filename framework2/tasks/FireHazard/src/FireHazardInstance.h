/* 
 * File:   FireHazardInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 4. May 2016
 */

#ifndef FIREHAZARDINSTANCE_H
#define FIREHAZARDINSTANCE_H

#include <chrono>
#include <string>
#include <memory>

#include "../../../src/CombinedTaskInstance.h"
#include "../../../src/TaskManager.h"
#include "../../../src/DataMessage.h"

struct ActuatorInfo {
    long long a_gateway_id;
    long a_device_euid;
    int a_module_id;
};

class FireHazardInstance: public CombinedTaskInstance
{
public:
    FireHazardInstance(long instance_id,
                       std::weak_ptr<TaskManager> owning_manager,
                       long device_euid);

    virtual ~FireHazardInstance();
    
    void run(DataMessage data_message) override;
    
    void run(std::chrono::system_clock::time_point activation_time) override;
    
private:
    double getModuleValue(int module_id, DataMessage data_message);
      
    short m_sos_sequence;
    
    bool m_sequence_running;
    
    ActuatorInfo getActuatorInfo();
            
};

#endif /* FIREHAZARDINSTANCE_H */
