/* 
 * File:   RefreshManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 21. January 2016
 */

#include "RefreshCheckManager.h"

#include "RefreshCheckInstance.h"

extern "C" {
    TaskManager* createTaskManager() {
        
        std::cout << "Creating RefreshTaskManager." << std::endl;
        return static_cast<TaskManager*>(new RefreshCheckManager());
    }
    
    void deleteTaskManager(TaskManager* manager) {
        std::cout << "Deleting RefreshTaskManager." << std::endl;
        delete static_cast<RefreshCheckManager*>(manager);
    }
}

RefreshCheckManager::RefreshCheckManager()
{
}

RefreshCheckManager::~RefreshCheckManager() {
    std::cout << "RefreshCheckManager::~RefreshCheckManager - entered." << std::endl;
    std::cout << "RefreshCheckManager::~RefreshCheckManager - finished." << std::endl;
}

/*
void RefreshCheckManager::createInstance(unsigned long user_id, unsigned int users_instance_personal_id) {
    //std::cout << "TimedAlgorithmManager::createInstance >> m_id: " << m_id << std::endl;
    m_task_instances.emplace(1, std::make_shared<RefreshCheckInstance>(user_id, users_instance_personal_id)});
    
    std::map<unsigned int  ID of instance , std::shared_ptr<TaskInstance>> m_task_instances;
    
    //m_instance_id_counter++;
}
 */


void RefreshCheckManager::createInstance(unsigned int user_id, unsigned short personal_id) {
    
    std::cout << "RefreshCheckManager::createInstance - entered." << std::endl;
    
    m_task_instances.emplace(1, std::make_shared<RefreshCheckInstance>(user_id, personal_id));
    
    std::cout << "RefreshCheckManager::createInstance - finished." << std::endl;
}

void RefreshCheckManager::deleteInstance() {

}
