/* 
 * File:   AlgorithmInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "AlgorithmInstance.h"

#include <string>

AlgorithmInstance::AlgorithmInstance(unsigned int manager_id, unsigned long instance_id, unsigned long user_id, unsigned int users_instance_personal_id, std::string text):
    m_manager_id(manager_id), m_instance_id(instance_id), m_text(text)
{
    // Insert owner information.
    m_owners.insert({user_id, users_instance_personal_id});
}

/*
AlgorithmInstance::AlgorithmInstance(const AlgorithmInstance& orig) {
}

AlgorithmInstance::~AlgorithmInstance() {
}
*/
