/* 
 * File:   TaskManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <vector> //std::vector

#include "UserMessages.h"
#include "BaseTaskManager.h"

class TaskManager: public BaseTaskManager, public std::enable_shared_from_this<TaskManager>
{
public:
    /**
     * Constructor of class TaskManager.
     */
    TaskManager();
    /**
     * Destructor of class TaskManager.
     */  
    virtual ~TaskManager();
    /**
     * Creates instance in database.
     * @param create_message Message with info about instance.
     * @return ID of instance from database.
     */
    virtual long createInstance(CreateMessage create_message) override;
    /**
     * Deletes instance from BAF and from database.
     * @param delete_message Message with info about which instance to delete.
     */
    virtual void deleteInstance(DeleteMessage delete_message) override;
    /**
     * Returns IDs of instances belonging to user and task given in message.
     * @param get_inst_ids_message Message containing user_id and task_id.
     * @return Vector with IDs of all instances owned by user. 
     */
    virtual std::vector<long> getInstanceIds(GetInstIdsMessage get_inst_ids_message) override;
    
    //virtual std::string givePermission(GivePermMessage give_perm_message);
 
    /**
     * To anable instance to delete itself.
     * @param instance_id ID of instance to delete from system.
     */
    void suicideInstance(long instance_id) override;
    /**
     * Deletes all instances from manager.     
     */
    void deleteAllInstances();
    /**
     * Prints debug information about all instances in manager.
     */
    void debugPrintTaskInstances();
};

#endif /* TASKMANAGER_H */
