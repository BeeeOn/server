/* 
 * File:   EventAlgorithmManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "EventAlgorithmManager.h"


EventAlgorithmManager::EventAlgorithmManager(MANAGER_TYPE type, std::string name):
    AlgorithmManager(type, name)
{
}
/*
EventAlgorithmManager::EventAlgorithmManager(const EventAlgorithmManager& orig) {
}
*/
EventAlgorithmManager::~EventAlgorithmManager() {
}

void EventAlgorithmManager::processDataMessage() {

}
