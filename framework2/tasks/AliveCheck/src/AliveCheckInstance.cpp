/* 
 * File:   AliveCheckInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 21. January 2016
 */

#include "AliveCheckInstance.h"

#include <chrono>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <memory>
#include <random>

#include <soci.h>

#include "../../../../Notificator/UriNotif.h"

#include "../../../src/DatabaseInterface.h"
#include "../../../src/Logger.h"
#include "../../../src/TaskManager.h"

AliveCheckInstance::AliveCheckInstance(long instance_id,
                                       std::weak_ptr<TaskManager> owning_manager,
                                       AliveCheckConfig parsed_config):
    TaskInstance(instance_id, owning_manager),
    TimedTaskInstance(instance_id, owning_manager),
    m_configuration(parsed_config)
{
    // Plan this instance randomly between zero and one minute.
    // This is because of reloading of instances from database,
    // because they are loaded at the same time.
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, 60);
    planActivationAfterSeconds(distr(eng));
}

AliveCheckInstance::~AliveCheckInstance()
{
}

void AliveCheckInstance::run(std::chrono::system_clock::time_point activation_time)
{
    logger.LOGFILE("alive_check", "INFO") << "AliveCheck instance with instance_id: "
            << m_instance_id << " has been run." << std::endl;
    
    planActivationAfterSeconds(30);
    
    runAliveCheck();
}

void AliveCheckInstance::runAliveCheck()
{
    double device_euid;
    long long measured_at;
    int refresh;

    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    soci::rowset<soci::row> rows = (sql->prepare << "SELECT device_euid, measured_at, refresh FROM device WHERE gateway_id = :gateway_id",
                                    soci::use(m_configuration.gateway_id, "gateway_id"));

    for (soci::rowset<soci::row>::const_iterator it = rows.begin(); it != rows.end(); ++it) {   
        
        soci::row const& row = *it;
        // Get values from rows.
        device_euid = row.get<double>(0);
        measured_at = row.get<long long>(1);
        refresh = row.get<int>(2);
        
        // Get timestamp of current time.
        int now_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        
        std::string status;
        *sql << "SELECT status FROM device WHERE device_euid = :device_euid",
                soci::into(status),
                soci::use(device_euid, "device_euid");
        
        // Three times refresh was chosen as the best value to test device availability.
        if ((measured_at + (3 * refresh)) < now_timestamp) {
            
            if (status != "unavailable") {
               
                *sql << "UPDATE device SET status = 'unavailable'::device_status WHERE device_euid = :device_euid",
                        soci::use(device_euid, "device_euid");

                // Send notification
                if (m_configuration.send_notif == 1) {
                    sendUnavailableNotification(static_cast<long>(device_euid));
                }
            }
            logger.LOGFILE("alive_check", "INFO") << "Instance: " << m_instance_id << " - Device with device_euid: "
                    << device_euid << " is unavailable." << std::endl;
        }
        else {
            if (status != "available") {
                
                *sql << "UPDATE device SET status = 'available'::device_status WHERE device_euid = :device_euid",
                    soci::use(device_euid, "device_euid");
            }
            logger.LOGFILE("alive_check", "INFO") << "Instance: " << m_instance_id
                    << " - Device with device_euid: " << device_euid << " is available." << std::endl;
        }
    }
}

void AliveCheckInstance::sendUnavailableNotification(long device_euid)
{
    std::string notification("Zařízení s euid: ");
    notification += std::to_string(device_euid);
    notification += " je nedostupné.";
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    long user_id;
    *sql << "SELECT user_id FROM user_instance WHERE intance_id = :instance_id",
            soci::use(m_instance_id, "instance_id"),
            soci::into(user_id);
    
    // Find all service_reference_ids binded with user.
    soci::rowset<soci::row> sri_rows = (sql->prepare << "SELECT service_reference_id "
                "FROM push_notification_service WHERE user_id = :user_id",
                 soci::use(user_id, "user_id"));
    
    std::vector<std::string> sr_ids;
    for (soci::rowset<soci::row>::const_iterator sri_it = sri_rows.begin(); sri_it != sri_rows.end(); ++sri_it) {   
    
        soci::row const& sri_row = *sri_it;
            // Get all service_reference_ids.
        sr_ids.push_back(sri_row.get<std::string>(0));
    }
    int now_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // URI notif is just placeholder until AliveCheck notification is specified.
    std::shared_ptr<UriNotif> notif = std::make_shared<UriNotif>(user_id, m_instance_id, now_timestamp, notification, "");
    // Send notifications.
    notif->sendGcm(&sr_ids);
}
