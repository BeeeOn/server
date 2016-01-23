/* 
 * File:   AlgorithmManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "AlgorithmManager.h"

AlgorithmManager::AlgorithmManager(int id, MANAGER_TYPE type, std::string name) :
    m_id(id), m_type(type), m_name(name)
{
}

AlgorithmManager::AlgorithmManager(const AlgorithmManager& orig) {
}

AlgorithmManager::~AlgorithmManager() {
}

