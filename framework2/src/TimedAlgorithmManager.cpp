/* 
 * File:   TimedAlgorithmManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TimedAlgorithmManager.h"

#include <iostream>
#include <memory>
#include <string>

#include "TimedAlgorithmInstance.h"

//std::map<unsigned long /*instance_id*/, std::shared_ptr<TimedAlgorithmInstance>> TimedAlgorithmManager::m_algorithm_instances;

TimedAlgorithmManager::TimedAlgorithmManager(MANAGER_TYPE type, std::string name) :
    AlgorithmManager(type, name)
{
    //std::cout << "TimedAlgorithmManager::TimedAlgorithmManager >> m_id: " << m_id << std::endl;
}
/*
TimedAlgorithmManager::TimedAlgorithmManager(const TimedAlgorithmManager& orig) {
}
*/
TimedAlgorithmManager::~TimedAlgorithmManager() {
}

void TimedAlgorithmManager::createInstance(unsigned long user_id, unsigned int users_instance_personal_id) {
    //std::cout << "TimedAlgorithmManager::createInstance >> m_id: " << m_id << std::endl;
    m_algorithm_instances.insert({m_instance_id_counter, std::make_shared<TimedAlgorithmInstance>(user_id, users_instance_personal_id)});
    
    m_instance_id_counter++;
}