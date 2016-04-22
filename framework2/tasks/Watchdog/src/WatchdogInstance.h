/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WatchdogInstance.h
 * Author: mrmaidx
 *
 * Created on 29. března 2016, 12:52
 */

#ifndef WATCHDOGINSTANCE_H
#define WATCHDOGINSTANCE_H

#include <chrono>
#include <string>
#include <memory>

#include "../../../src/TriggerTaskInstance.h"
#include "../../../src/DataMessage.h"
#include "../../../src/TaskManager.h"

class WatchdogInstance: public TriggerTaskInstance
{
public:
    WatchdogInstance(long instance_id, std::weak_ptr<TaskManager> owning_manager, long device_euid);
    //WatchdogInstance(const WatchdogInstance& orig);
    virtual ~WatchdogInstance();

    void run(DataMessage data_message) override;

private:
    /** Variable indicates, that instance received atleast one data message. */
    bool m_received_data_once;
    
    /** Indicated if watchdog operator was met and watchdog executed its function. */
    //bool m_can_run;
    
    /** Stores time when watchdog executed last time. */
    //std::chrono::system_clock::time_point m_last_run_time;
    
    /** Last received value from module. */
    double m_last_received_value;
    
};

#endif /* WATCHDOGINSTANCE_H */

