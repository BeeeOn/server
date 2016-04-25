/* 
 * File:   AliveCheckManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 21. January 2016
 */

#include "AliveCheckManager.h"

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <soci.h>
#include <iostream>
#include <stdexcept>

#include "AliveCheckInstance.h"
#include "../../../src/DatabaseInterface.h"
#include "../../../src/Logger.h"

extern "C" {
    std::shared_ptr<TaskManager> createTaskManager()
    {
        logger.LOGFILE("alive_check", "INFO") << "AliveCheck manager was created." << std::endl;
        return std::make_shared<AliveCheckManager>();
    }
}

AliveCheckManager::AliveCheckManager()
{
}

AliveCheckManager::~AliveCheckManager()
{
}

void AliveCheckManager::createConfiguration(long instance_id, std::map<std::string, std::string> config)
{
    // Parse received configuration.
    AliveCheckConfig parsed_config = parseConfiguration(config);
    
    // Store configuration to database.
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "INSERT INTO task_alive_check(instance_id, send_notif, gateway_id) VALUES(:instance_id, :send_notif, :gateway_id)",
            soci::use(instance_id, "instance_id"),
            soci::use(parsed_config.gateway_id, "gateway_id"),
            soci::use(parsed_config.send_notif, "send_notif");
    
    
    
    // Create new instance of AliveCheck in system.
    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    m_task_instances.emplace(instance_id, std::make_shared<AliveCheckInstance>(instance_id, shared_from_this(), parsed_config));
    
    
    logger.LOGFILE("alive_check", "INFO") << "New instance of AliveCheck was created: instance_id: "
            << instance_id << std::endl;
}

void AliveCheckManager::changeConfiguration(ChangeMessage change_message)
{
    std::shared_ptr<AliveCheckInstance> instance;
    
    auto instance_it = m_task_instances.find(change_message.instance_id);
    if (instance_it == m_task_instances.end()) {
        throw std::runtime_error(std::string("Instance with ID: ")
              + std::to_string(change_message.instance_id)
              + " was not found in BAF system.");
    }
    else {
        instance = std::dynamic_pointer_cast<AliveCheckInstance>(instance_it->second);
    }
    
    // Insert changes to database.
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    soci::transaction tr(*sql);
    
    // Try to find gateway_id member.
    auto gateway_id_it = change_message.config.find("gateway_id");
    if (gateway_id_it != change_message.config.end()) {
        
        // Convert string to long long.
        long long gateway_id = std::stoll(gateway_id_it->second); 
        // Insert to database.
        *sql << "UPDATE task_alive_check SET gateway_id = :gateway_id WHERE instance_id = :instance_id",
                soci::use(gateway_id, "gateway_id"),
                soci::use(change_message.instance_id, "instance_id");
        // Change local configuration.
        instance->m_configuration.gateway_id = gateway_id;
    }
    
    auto send_notif_it = change_message.config.find("send_notif");
    if (send_notif_it != change_message.config.end()) {
        
        // Converts send_notif to short.
        // send_notif must be 1 for true or 0 for false.
        short send_notif = std::stoi(send_notif_it->second);
        // Insert to database.
        *sql << "UPDATE task_alive_check SET send_notif = :send_notif WHERE instance_id = :instance_id",
                soci::use(send_notif, "send_notif"),
                soci::use(change_message.instance_id, "instance_id");
        // Change local configuration.
        instance->m_configuration.send_notif = send_notif;
    }
    tr.commit();
}

std::map<std::string, std::string> AliveCheckManager::getConfiguration(GetConfMessage get_conf_message)
{
    AliveCheckConfig alive_config;
    std::map<std::string, std::string> config_map;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "SELECT gateway_id, send_notif FROM task_alive_check WHERE instance_id = :instance_id",
            soci::use(get_conf_message.instance_id, "instance_id"),
            soci::into(alive_config.gateway_id),
            soci::into(alive_config.send_notif);

    config_map["gateway_id"] = std::to_string(alive_config.gateway_id);
    config_map["send_notif"] = std::to_string(alive_config.send_notif);
    return config_map;
}

void AliveCheckManager::reloadInstances(unsigned int task_id)
{
    long instance_id;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    soci::rowset<soci::row> rows = (sql->prepare << "SELECT instance_id FROM instance WHERE task_id = :task_id",
                                    soci::use(task_id, "task_id"));

    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    for (soci::rowset<soci::row>::const_iterator it = rows.begin(); it != rows.end(); ++it) {   
        
        soci::row const& row = *it;
        instance_id = row.get<int>(0);
        
        // Get configuration from database.
        AliveCheckConfig configuration;
        SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
        *sql << "SELECT gateway_id, send_notif FROM task_alive_check WHERE instance_id = :instance_id",
                soci::into(configuration.gateway_id),
                soci::into(configuration.send_notif),
                soci::use(instance_id, "instance_id");

        // Create instance in system.
        m_task_instances.emplace(instance_id, std::make_shared<AliveCheckInstance>(instance_id, shared_from_this(), configuration));

        logger.LOGFILE("alive_check", "INFO") << "Instance of AliveCheck was reloaded: instance_id: "
                << instance_id << std::endl;
    }
}

AliveCheckConfig AliveCheckManager::parseConfiguration(std::map<std::string, std::string> configuration)
{
    AliveCheckConfig parsed_config;
    
    auto gateway_id_it = configuration.find("gateway_id");
    if (gateway_id_it == configuration.end()) {
        
        logger.LOGFILE("alive_check", "ERROR") << "Received configuration doesn't "
                "have member gateway_id." << std::endl;
        throw std::runtime_error("Could not parse received configuration.");
    }
    else {
        parsed_config.gateway_id = std::stoll(gateway_id_it->second);
    }

    auto send_notif_it = configuration.find("send_notif");
    if (send_notif_it == configuration.end()) {
        
        logger.LOGFILE("alive_check", "ERROR") << "Received configuration doesn't "
                "have member send_notif." << std::endl;
        throw std::runtime_error("Could not parse received configuration.");
    }
    else {
        parsed_config.send_notif = std::stoll(send_notif_it->second);
    }
    return parsed_config;
}
