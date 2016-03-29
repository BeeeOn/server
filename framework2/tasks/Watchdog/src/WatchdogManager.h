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
    unsigned long long device_euid;
    int module_id;
    std::string comp_operator;
    double value;
    std::string notification_text;
};


class WatchdogManager: public TaskManager
{
public:
    WatchdogManager();
    //WatchdogManager(const WatchdogManager& orig);
    virtual ~WatchdogManager();
    
    void createInstance(unsigned int instance_id, std::map<std::string, std::string> configuration) override;

    //void storeConfiguration(std::map<std::string, std::string> configuration) override;

    void updateConfiguration(unsigned int instance_id, std::map<std::string, std::string> configuration) override;
private:

    WatchdogConfig parseConfiguration(std::map<std::string, std::string> configuration);
};

#endif /* WATCHDOGMANAGER_H */

