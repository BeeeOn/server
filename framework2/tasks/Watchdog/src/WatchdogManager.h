/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WatchdogManager.h
 * Author: mrmaidx
 *
 * Created on 29. března 2016, 12:52
 */

#ifndef WATCHDOGMANAGER_H
#define WATCHDOGMANAGER_H

#include "../../../src/TaskManager.h"

#include <iostream>
#include <map>
#include <string>

struct WatchdogConfig {
    long device_euid;
    int module_id;
    std::string comp_operator;
    double value;
    std::string notification;
};


class WatchdogManager: public TaskManager
{
public:
    WatchdogManager();
    //WatchdogManager(const WatchdogManager& orig);
    ~WatchdogManager();
    
    void createConfiguration(long instance_id, std::map<std::string, std::string> config) override;

    void changeConfiguration(ChangeMessage change_message) override;
    
    std::map<std::string, std::string> getConfiguration(GetConfMessage get_conf_message) override;
    
    void reloadInstances(unsigned int task_id) override;

private:

    WatchdogConfig parseConfiguration(std::map<std::string, std::string> configuration);
};

#endif /* WATCHDOGMANAGER_H */

