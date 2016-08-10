/*
 * File:   BaseTaskManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 12. April 2016
 */

#include "core/BaseTaskManager.h"

#include <stdexcept>

BaseTaskManager::BaseTaskManager()
{
}

BaseTaskManager::~BaseTaskManager()
{
}

std::string BaseTaskManager::getData(GetDataMessage get_data_message)
{
	throw std::runtime_error("Get data functionality is not implemeted in this task.");
}
