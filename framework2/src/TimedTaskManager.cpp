/* 
 * File:   TimedAlgorithmManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TimedTaskManager.h"

#include <iostream>
#include <memory>
#include <string>

#include "TimedTaskInstance.h"

//std::map<unsigned long /*instance_id*/, std::shared_ptr<TimedAlgorithmInstance>> TimedAlgorithmManager::m_algorithm_instances;

TimedTaskManager::TimedTaskManager(MANAGER_TYPE type, std::string name) :
    TaskManager(type, name)
{
    //std::cout << "TimedAlgorithmManager::TimedAlgorithmManager >> m_id: " << m_id << std::endl;
}
/*
TimedAlgorithmManager::TimedAlgorithmManager(const TimedAlgorithmManager& orig) {
}
*/
TimedTaskManager::~TimedTaskManager() {
}

void TimedTaskManager::createInstance(unsigned long user_id, unsigned int users_instance_personal_id) {
    //std::cout << "TimedAlgorithmManager::createInstance >> m_id: " << m_id << std::endl;
    m_algorithm_instances.insert({m_instance_id_counter, std::make_shared<TimedTaskInstance>(user_id, users_instance_personal_id)});
    
    m_instance_id_counter++;
}