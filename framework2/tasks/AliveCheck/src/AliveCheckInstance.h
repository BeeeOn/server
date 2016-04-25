/* 
 * File:   AliveCheckInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#ifndef ALIVECHECKINSTANCE_H
#define ALIVECHECKINSTANCE_H

#include <chrono>
#include <memory>

#include "../../../src/TimedTaskInstance.h"
#include "../../../src/TaskManager.h"

#include "AliveCheckManager.h"

class AliveCheckInstance: public TimedTaskInstance
{
public:
    /**
     * Constructor of class AliveCheckInstance.
     */
    AliveCheckInstance(long instance_id,
                       std::weak_ptr<TaskManager> owning_manager,
                       AliveCheckConfig parsed_config);
    /**
     * Destructor of class AliveCheckInstance.
     */
    virtual ~AliveCheckInstance();
    /**
     * Entry point for task main algorithm.
     * @param activation_time Time at which was instance activated.
     */
    void run(std::chrono::system_clock::time_point activation_time) override;
    /**
     * Object with stored configuration.
     * Serves to reduce load on database.
     */
    AliveCheckConfig m_configuration;
    
private:
    /**
     * Prevents instance to send more than
     * one notification when device is unavailable.
     */
    bool m_send_notification;
    /**
     * Contains main logic of task.
     */
    void runAliveCheck();
    /**
     * Sends notification to all users who has gateway with connected device.
     * @param now_timestamp Timestamp of event.
     * @param device_euid EUID of device which is unavailable.
     */
    void sendUnavailableNotification(long now_timestamp, long device_euid);
    
};
#endif /* ALIVECHECKINSTANCE_H */
