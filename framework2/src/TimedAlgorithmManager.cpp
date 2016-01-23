/* 
 * File:   TimedAlgorithmManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TimedAlgorithmManager.h"

TimedAlgorithmManager::TimedAlgorithmManager(int id, MANAGER_TYPE type, std::string name) :
    AlgorithmManager(id, type, name)
{
}
/*
TimedAlgorithmManager::TimedAlgorithmManager(const TimedAlgorithmManager& orig) {
}
*/
TimedAlgorithmManager::~TimedAlgorithmManager() {
}

