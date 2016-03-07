/* 
 * File:   TestManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 26. January 2016
 */

#include "TestManager.h"

#include "TestInstance.h"

TestManager::TestManager(MANAGER_TYPE type, std::string name) :
    TimedTaskManager(type, name)
{
}

TestManager::~TestManager()
{
}

void TestManager::createInstance(unsigned long user_id, unsigned int users_instance_personal_id) {
    //std::cout << "TimedAlgorithmManager::createInstance >> m_id: " << m_id << std::endl;
    m_algorithm_instances.insert({m_instance_id_counter, std::make_shared<TestInstance>(user_id, users_instance_personal_id)});
    
    m_instance_id_counter++;
}