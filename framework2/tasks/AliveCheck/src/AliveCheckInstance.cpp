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

#include <soci.h>

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
    planActivationNow();
}

AliveCheckInstance::~AliveCheckInstance()
{
}

void AliveCheckInstance::run(std::chrono::system_clock::time_point activation_time)
{
    logger.LOGFILE("alive_check", "INFO") << "AliveCheck instance with instance_id: "
            << m_instance_id << " has been run." << std::endl;
    
    planActivationAfterSeconds(5);
    
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
        long now_timestamp = std::chrono::seconds(std::time(NULL)).count();
        
        // Three times refresh was chosen as the best value to test device availability.
        if ((measured_at + (3 * refresh)) < now_timestamp) {
            
            *sql << "UPDATE device SET status = 'unavailable'::device_status WHERE device_euid = :device_euid",
                    soci::use(device_euid, "device_euid");
            
            logger.LOGFILE("alive_check", "INFO") << "Device with device_euid: " << device_euid
                    << " is now unavailable." << std::endl;
        }
        /*
        else {
            *sql << "UPDATE device SET status = 'available'::device_status WHERE device_euid = :device_euid",
                    soci::use(device_euid, "device_euid");
        }
        */
    }
    
    /*
    
    long long gateway_id;
    int send_notif;
    
    // Get gateway_id from table with configuration. 
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "SELECT gateway_id, send_notif FROM task_alive_check WHERE instance_id = :instance_id",
            soci::into(gateway_id), soci::into(send_notif), soci::use(m_instance_id, "instance_id");
    
    std::cout << "instance_id" << m_instance_id << "DATABASE: gateway_id: " << gateway_id << " | send_notif: " << send_notif << std::endl;
    
    std::cout << "instance_id" << m_instance_id << "LOCAL: gateway_id: " << m_configuration.gateway_id << " | send_notif: " << m_configuration.send_notif << std::endl;
    */
    
    /*
    std::vector<unsigned long long> device_euid(100);
    std::vector<unsigned long long> measured_at(100);
    std::vector<int> refresh(100);
    *sql << "SELECT device_euid, measured_at, refresh FROM device WHERE gateway_id = :gateway_id",
            soci::into(device_euid), soci::into(measured_at), soci::into(refresh), soci::use(gateway_id, "gateway_id");
    
    long int now_timestamp = std::chrono::seconds(std::time(NULL)).count();
    std::cout << "now: " << now_timestamp << std::endl;
    
    std::cout << "DEVICES:" << std::endl;
    for (int i = 0; i < measured_at.size(); i++) {
        std::cout << "measured_at: " << measured_at[i] << " | refresh: " << refresh[i] << std::endl;
        if (measured_at[i] + (3 * refresh[i]) > now_timestamp) {
            
            *sql << "UPDATE device SET status = 'available'::device_status WHERE device_euid = :device_euid",
                    soci::use(device_euid[i], "device_euid");
        }
        else {
            *sql << "UPDATE device SET status = 'unavailable'::device_status WHERE device_euid = :device_euid",
                    soci::use(device_euid[i], "device_euid"); 
        }
    }
    
    std::cout << "ID instance: " << m_instance_id << std::endl;
    //*sql << "SELECT notifications FROM task_alive_check WHERE instance_id = :"
    */
    /*
    session sql(postgresql, "port = '5432' dbname = 'home7' user = 'uiserver7' password = '1234' connect_timeout = '3'");
  
    std::cout << "Update devices ... ";
    
    sql << "UPDATE device SET status = CASE measured_at + 3 * get_refresh(gateway_id, device_euid) > extract(epoch from now()) "
           "WHEN true THEN 'available'::device_status "                                                                                                                                                  
           "WHEN false THEN 'unavailable'::device_status " 
           "ELSE 'available'::device_status "
           "END;";
    
    std::cout << "done" << std::endl;
    */
}
     