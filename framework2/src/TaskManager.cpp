/* 
 * File:   AlgorithmManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TaskManager.h"

#include <iostream>


TaskManager::TaskManager(MANAGER_TYPE type, std::string name):
    m_type(type), m_name(name)
{
}

TaskManager::TaskManager(const TaskManager& orig) {
}

TaskManager::~TaskManager() {
}