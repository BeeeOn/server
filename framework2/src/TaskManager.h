/* 
 * File:   TaskManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <map>
#include <memory> //std::shared_ptr
//#include <utility> // std::pair

#include "ConfigMessage.h"
#include "TaskInstance.h"

class TaskManager {
public:
    
    TaskManager();
    
    virtual ~TaskManager();
    
    /**
     * Crates new instance of instance.
     * @param user_id ID of a user creating an instance.
     * @param personal_id ID of instance relative to instances of task already created by user (user can run more instances of one task). 
     */
    virtual void createInstance(unsigned int instance_id, std::map<std::string, std::string> configuration) = 0;
    
    // Prvotni ulozeni konfigurace ulohy.
    //virtual void storeConfiguration(std::map<std::string, std::string> configuration) = 0;
    
    // Zmena uz existujici konfigurace.
    virtual void changeConfiguration(unsigned int instance_id, std::map<std::string /*name*/, std::string /*value*/> configuration) = 0;
   
    // Smazani konfigurace.
    virtual void deleteConfiguration(unsigned int user_id, unsigned short personal_id) = 0;
    
    // Smazani instance.
    void deleteInstance(unsigned int user_id, unsigned short personal_id);
    
    /**
     * 
     * @param config_message
     */
    virtual void makeNewInstance(ConfigMessage config_message) ;
    
    /**
     * Checks if instance exists in database and BAF system.
     * This function is little hacked through exception, it should be probably done without it.
     * @param config_message Configuration data.
     * @return True if found, false otherwise.
     */
    bool checkInstanceExistence(ConfigMessage config_message);
    
    unsigned int getInstanceId(unsigned int user_id, unsigned short relative_id);
    
protected:

    /**
     * Stores all created instances of this task. TaskInstance is only an interface so stored instances 
     * are either derived from TimedTaskInstance, TriggerTaskInstance or CombinedTaskInstance.
     */
    std::map<unsigned int /* ID of instance */, std::shared_ptr<TaskInstance>> m_task_instances;
    
    //TaskManager(MANAGER_TYPE type, std::string name);
    
    //TaskManager(const TaskManager& orig);
    //virtual ~TaskManager();
        
    //void processConfigMessage();
    
    // Getters.
    //unsigned int getId() const {return m_id;}
    //std::string getName() const {return m_name;};
    //MANAGER_TYPE getType() const {return m_type;};    
    
    // Setters.
    //void setType(MANAGER_TYPE type) {m_type = type;}
    
    // Creates an instance of algorithm.
    //virtual void createInstance();
    
//protected:
    // Variable for generating unique ids for instances.  
    //unsigned long m_instance_id_counter = 1;
    
    // Id of manager.
    //unsigned int m_id;
    
    //std::string m_name;
    
    //MANAGER_TYPE m_type;
    

};

#endif /* TASKMANAGER_H */