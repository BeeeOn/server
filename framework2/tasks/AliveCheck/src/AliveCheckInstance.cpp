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
#include "../../../src/TaskManager.h"

AliveCheckInstance::AliveCheckInstance(unsigned int instance_id, std::weak_ptr<TaskManager> owning_manager):
    TimedTaskInstance(instance_id, owning_manager)
{
    planActivationNow();
}

AliveCheckInstance::~AliveCheckInstance()
{
}

void AliveCheckInstance::run(std::chrono::system_clock::time_point activation_time)
{
    std::cout << "AliveCheckInstance::run() - enter" << std::endl;

    planActivationAfterSeconds(5);
    
    executeRefresh();
    std::cout << "AliveCheckInstance::run() - leave" << std::endl;
}

void AliveCheckInstance::executeRefresh()
{
    // Suicide.
    //deleteItself();
    //return;
    
    unsigned long long gateway_id;
    int send_notif;
    
    // Get gateway_id from table with configuration. 
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "SELECT gateway_id, send_notif FROM task_alive_check WHERE instance_id = :instance_id",
            soci::into(gateway_id), soci::into(send_notif), soci::use(m_instance_id, "instance_id");
    
    std::cout << "gateway_id: " << gateway_id << " | send_notif: " << send_notif << std::endl;
    
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
     