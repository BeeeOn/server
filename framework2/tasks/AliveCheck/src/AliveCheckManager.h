/* 
 * File:   AliveCheckManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#ifndef ALIVECHECKMANAGER_H
#define ALIVECHECKMANAGER_H

#include <iostream>
#include <map>
#include <string>

#include "../../../src/UserMessages.h"
#include "../../../src/TaskManager.h"

struct AliveCheckConfig {
    short send_notif;
    unsigned int gateway_id;
};

class AliveCheckManager: public TaskManager
{
public:
    
    AliveCheckManager();
              
    void createConfiguration(long instance_id, std::map<std::string, std::string> config) override;

    void changeConfiguration(ChangeMessage change_message) override;
    
    std::map<std::string, std::string> getConfiguration(GetConfMessage get_conf_message) override;    
    
    void reloadInstances(unsigned int task_id) override;

    
    //void createInstance(unsigned int instance_id, std::map<std::string, std::string> configuration) override;

    //void storeConfiguration(std::map<std::string, std::string> configuration) override;

    //void updateConfiguration(unsigned int instance_id, std::map<std::string, std::string> configuration) override;

    //void deleteConfiguration(unsigned int user_id, unsigned short personal_id) override;
    
    ~AliveCheckManager();
private:

    AliveCheckConfig parseConfiguration(std::map<std::string, std::string> configuration);
};
#endif /* ALIVECHECKMANAGER_H */

