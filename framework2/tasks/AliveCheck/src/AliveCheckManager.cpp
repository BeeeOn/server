/* 
 * File:   AliveCheckManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 21. January 2016
 */

#include "AliveCheckManager.h"

#include <map>
#include <string>
#include <soci.h>

#include "../../../src/DatabaseInterface.h"
#include "AliveCheckInstance.h"

extern "C" {
    TaskManager* createTaskManager() {
        
        std::cout << "Creating AliveCheckManager." << std::endl;
        return static_cast<TaskManager*>(new AliveCheckManager());
    }
    
    void deleteTaskManager(TaskManager* manager) {
        std::cout << "Deleting AliveCheckManager." << std::endl;
        delete static_cast<AliveCheckManager*>(manager);
    }
}

AliveCheckManager::AliveCheckManager()
{
}

AliveCheckManager::~AliveCheckManager()
{
    //std::cout << "RefreshCheckManager::~RefreshCheckManager." << std::endl;
}

void AliveCheckManager::createInstance(unsigned int instance_id, std::map<std::string, std::string> configuration)
{
    std::cout << "Instert instance with instance_id: " << instance_id << " into database." << std::endl;
    AliveCheckConfig conf = parseConfiguration(configuration);
    
    // Configuration in special database table.
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "INSERT INTO task_alive_check(instance_id, notifications, gateway_id) VALUES(:instance_id, :notifications, :gateway_id)",
            soci::use(instance_id, "instance_id"), soci::use(conf.gateway_id, "gateway_id"), soci::use(conf.notifications, "notifications");
            
            /*"INSERT INTO task_alive_check(instance_id, notifications, gateway_id) VALUES(:instance_id, :notifications, :gateway_id)",
            
            
            soci::use(notifications, "notifications");
    */
    std::cout << "Emplace instance with instance_id: " << instance_id << " into BAF." << std::endl;
    m_task_instances.emplace(instance_id, std::make_shared<AliveCheckInstance>(instance_id));
}
/*
void RefreshCheckManager::storeConfiguration(std::map<std::string, std::string> configuration)
{

}
*/

void AliveCheckManager::updateConfiguration(unsigned int instance_id, std::map<std::string, std::string> configuration)
{
    AliveCheckConfig conf = parseConfiguration(configuration);
    // Update configuration of instance with given instance_id.
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "UPDATE task_alive_check SET notifications = :notifications, gateway_id = :gateway_id WHERE instance_id = :instance_id",
            soci::use(conf.notifications, "notifications"), soci::use(conf.gateway_id, "gateway_id"),
            soci::use(instance_id, "instance_id");
}
/*
void AliveCheckManager::deleteConfiguration(unsigned int user_id, unsigned short personal_id) {

}
*/

AliveCheckConfig AliveCheckManager::parseConfiguration(std::map<std::string, std::string> configuration)
{
    AliveCheckConfig conf;
    conf.gateway_id = std::stoi(configuration.find("gateway_id")->second);
    conf.notifications = std::stoi(configuration.find("notifications")->second);
    
    return conf;
}
