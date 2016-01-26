/* 
 * File:   AlgorithmInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "AlgorithmInstance.h"

#include <string>

AlgorithmInstance::AlgorithmInstance(unsigned long user_id, unsigned int users_instance_personal_id)
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
