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

class AliveCheckInstance: public TimedTaskInstance
{
public:
    AliveCheckInstance(unsigned int instance_id, std::weak_ptr<TaskManager> owning_manager);
    
    virtual ~AliveCheckInstance();
    
    void run(std::chrono::system_clock::time_point activation_time) override;
    
private:
    void executeRefresh();
};
#endif /* ALIVECHECKINSTANCE_H */

