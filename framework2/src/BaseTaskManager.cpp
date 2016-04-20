/* 
 * File:   BaseTaskManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 12. April 2016
 */

#include "BaseTaskManager.h"

#include <stdexcept>
#include <iostream>

BaseTaskManager::BaseTaskManager()
{
}


BaseTaskManager::~BaseTaskManager()
{
    std::cout << "BaseTaskManager::~BaseTaskManager" << std::endl;
}

std::string BaseTaskManager::getData(GetDataMessage get_data_message)
{
    throw std::runtime_error("getData() function not implemeted.");
}