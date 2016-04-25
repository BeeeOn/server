/* 
 * File:   AliveCheckInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#ifndef ALIVECHECKINSTANCE_H
#define ALIVECHECKINSTANCE_H

#include <chrono>
#include <string>
#include <memory>

#include "../../../src/TimedTaskInstance.h"
#include "../../../src/TaskManager.h"
#include "AliveCheckManager.h"

class AliveCheckInstance: public TimedTaskInstance
{
public:
    AliveCheckInstance(long instance_id,
                       std::weak_ptr<TaskManager> owning_manager,
                       AliveCheckConfig parsed_config);
    
    virtual ~AliveCheckInstance();
    
    void run(std::chrono::system_clock::time_point activation_time) override;
    
    /**
     * Object with stored configuration.
     * Serves to reduce load on database.
     */
    AliveCheckConfig m_configuration;
    
private:
    
    void runAliveCheck();
    
    
};
#endif /* ALIVECHECKINSTANCE_H */

