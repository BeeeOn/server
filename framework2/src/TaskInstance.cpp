/* 
 * File:   TaskInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TaskInstance.h"

#include <string>

TaskInstance::TaskInstance(unsigned long user_id, unsigned int users_instance_personal_id)
{
    // Insert owner information.
    m_owners.insert({user_id, users_instance_personal_id});
}

/*
AlgorithmInstance::AlgorithmInstance(const AlgorithmInstance& orig) {
}
*/
TaskInstance::~TaskInstance() {
}

