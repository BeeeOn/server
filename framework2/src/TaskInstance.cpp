/* 
 * File:   TaskInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TaskInstance.h"

#include <string>

TaskInstance::TaskInstance(unsigned int instance_id):
    m_instance_id(instance_id)
{
    // Save owner information to database.
}

TaskInstance::~TaskInstance() {
    
    // Delete instance from database.
    
}