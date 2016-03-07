/* 
 * File:   RefreshManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 21. January 2016
 */

#include "RefreshCheckManager.h"

#include "RefreshCheckInstance.h"

extern "C" {
    TimedTaskManager* loadManager(MANAGER_TYPE type, std::string name) {
        return static_cast<TimedTaskManager*>(new RefreshCheckManager(type, name));
    }
    void deleteManager(TimedTaskManager* manager) {
        delete static_cast<RefreshCheckManager*>(manager);
    }
}

RefreshCheckManager::RefreshCheckManager(MANAGER_TYPE type, std::string name) :
    TimedTaskManager(type, name)
{
}
/*
RefreshManager::RefreshManager(const RefreshManager& orig) {
}
*/
RefreshCheckManager::~RefreshCheckManager() {
}

void RefreshCheckManager::createInstance(unsigned long user_id, unsigned int users_instance_personal_id) {
    //std::cout << "TimedAlgorithmManager::createInstance >> m_id: " << m_id << std::endl;
    m_algorithm_instances.insert({m_instance_id_counter, std::make_shared<RefreshCheckInstance>(user_id, users_instance_personal_id)});
    
    m_instance_id_counter++;
}