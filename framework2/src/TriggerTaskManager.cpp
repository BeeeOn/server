/* 
 * File:   EventAlgorithmManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TriggerTaskManager.h"


TriggerTaskManager::TriggerTaskManager(MANAGER_TYPE type, std::string name):
    TaskManager(type, name)
{
}
/*
EventAlgorithmManager::EventAlgorithmManager(const EventAlgorithmManager& orig) {
}
*/
TriggerTaskManager::~TriggerTaskManager() {
}

void TriggerTaskManager::processDataMessage() {

}
