/* 
 * File:   BaseTaskManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 12. April 2016
 */

#include "BaseTaskManager.h"

BaseTaskManager::BaseTaskManager()
{
}


BaseTaskManager::~BaseTaskManager()
{
}

std::string BaseTaskManager::getData(GetDataMessage get_data_message)
{
    return "This task doesn't return any data.";
}