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
//#include "TaskInstance.h"

class TaskManager: public BaseTaskManager
{
public:
    TaskManager();
    
    virtual ~TaskManager();
    
    /**
     * Creates instance in database.
     * @param create_message Message with info about instance.
     * @return ID of instance from database.
     */
    virtual long createInstance(CreateMessage create_message);
    
    /**
     * Deletes instance from BAF and from database.
     * @param delete_message Message with info about which instance to delete.
     */
    virtual void deleteInstance(DeleteMessage delete_message);
    
    /**
     * Returns IDs of instances belonging to user and task given in message.
     * @param get_inst_ids_message Message containing user_id and task_id.
     * @return Vector with IDs of all instances owned by user. 
     */
    virtual std::vector<std::string> getInstanceIds(GetInstIdsMessage get_inst_ids_message);
    
    //virtual std::string givePermission(GivePermMessage give_perm_message);
};

#endif /* TASKMANAGER_H */
