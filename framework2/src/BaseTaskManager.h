/* 
 * File:   BaseTaskManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 12. April 2016
 */

#ifndef BASETASKMANAGER_H
#define BASETASKMANAGER_H

#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "UserMessages.h"
#include "TaskInstance.h"

class BaseTaskManager
{
public:
    /**
     * Constructor of class BaseTaskManager.
     */
    BaseTaskManager();
    /**
     * Destructor of class BaseTaskManager.
     */
    virtual ~BaseTaskManager();
    /**
     * Creates instance in database.
     * @param create_message Message with info about instance.
     * @return ID of instance from database.
     */
    virtual long createInstance(CreateMessage create_message) = 0;
    /**
     * Stores configuration of given instance in database.
     * @param instance_id ID of instance which configuration should be stored.
     * @param config Configuration to store.
     */
    virtual void createConfiguration(long instance_id, std::map<std::string, std::string> config) = 0;
    /**
     * Changes configuration of instance.
     * @param change_message Message with info about instance and changed configuration.
     */
    virtual void changeConfiguration(ChangeMessage change_message) = 0;
    /**
     * Deletes instance from BAF and from database.
     * @param delete_message Message with info about which instance to delete.
     */
    virtual void deleteInstance(DeleteMessage delete_message) = 0;
    /**
     * Gets configuration of instance from database.
     * @param get_conf_message Message with info about instance.
     * @return Map with instance configuration.
     */
    virtual std::map<std::string, std::string> getConfiguration(GetConfMessage get_conf_message) = 0;
    /**
     * Returns any data. Manager implementing this function must know how to process received message.
     * @param get_data_message Message with info about instance and paramaters defining which data to return.
     * @return Message containing any data. UI must know how to process them.
     */
    virtual std::string getData(GetDataMessage get_data_message);
    /**
     * Returns IDs of instances belonging to user and task given in message.
     * @param get_inst_ids_message Message containing user_id and task_id.
     * @return Vector with IDs of all instances owned by user. 
     */
    virtual std::vector<long> getInstanceIds(GetInstIdsMessage get_inst_ids_message) = 0;
    /**
     * Function which enables instance to delete itself.
     * @param instance_id ID of instance to delete from system.
     */
    virtual void suicideInstance(long instance_id) = 0;
    /**
     * On start of BAF reloads instances from database.
     * In case when BAF needs to be restarted.
     */
    virtual void reloadInstances(unsigned int task_id) = 0;
    
protected:
    /**
     * Stores all created instances of this task. TaskInstance is only an interface so stored instances 
     * are either derived from TimedTaskInstance, TriggerTaskInstance or CombinedTaskInstance.
     */
    std::map<long /* ID of instance */, std::shared_ptr<TaskInstance>> m_task_instances;
    /**
     * Mutex to protect m_task_instances container.
     */
    std::mutex m_task_instances_mx;
};

#endif /* BASETASKMANAGER_H */
