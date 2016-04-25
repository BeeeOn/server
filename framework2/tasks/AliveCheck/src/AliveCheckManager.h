/* 
 * File:   AliveCheckManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#ifndef ALIVECHECKMANAGER_H
#define ALIVECHECKMANAGER_H

#include <map>
#include <string>

#include "../../../src/UserMessages.h"
#include "../../../src/TaskManager.h"

struct AliveCheckConfig {
    short send_notif;
    long long gateway_id;
};

class AliveCheckManager: public TaskManager
{
public:
    /**
     * Constructor of class AliveCheckManager().
     */
    AliveCheckManager();
    /**
     * Destructor of class AliveCheckManager().
     */
    ~AliveCheckManager();
    /**
     * Stores configuration of given instance in database.
     * @param instance_id ID of instance which configuration should be stored.
     * @param config Configuration to store.
     */
    void createConfiguration(long instance_id, std::map<std::string, std::string> config) override;
    /**
     * Changes configuration of instance.
     * @param change_message Message with info about instance and changed configuration.
     */
    void changeConfiguration(ChangeMessage change_message) override;
    /**
     * Gets configuration of instance from database.
     * @param get_conf_message Message with info about instance.
     * @return Map with instance configuration.
     */
    std::map<std::string, std::string> getConfiguration(GetConfMessage get_conf_message) override;    
    /**
     * On start of BAF reloads instances from database.
     * In case when BAF needs to be restarted.
     */
    void reloadInstances(unsigned int task_id) override;
    
private:
    /**
     * Parses configuration into AliveCheckConfig object.
     * @param configuration Map of configuration to parse.
     * @return Parsed AliveCheckConfig object.
     */
    AliveCheckConfig parseConfiguration(std::map<std::string, std::string> configuration);
};
#endif /* ALIVECHECKMANAGER_H */
