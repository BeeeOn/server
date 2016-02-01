/* 
 * File:   ManagerLoader.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */



// Algorithm includes.
//#include "../algorithms/RefreshManager.h"
//#include "../algorithms/WatchdogManager.h"
//#include "Logger.h"

#include "ManagerLoader.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "TimedAlgorithmManager.h"
#include "../algorithms/RefreshManager.h"
#include "../algorithms/TestManager.h"

ManagerLoader::ManagerLoader() {
}

ManagerLoader::ManagerLoader(const ManagerLoader& orig) {
}

ManagerLoader::~ManagerLoader() {
}

void ManagerLoader::loadAlgorithmManagers() {
    
    // Create refresh algorithm manager.
    m_timed_managers.emplace(1, std::make_shared<RefreshManager>(MANAGER_TYPE::TIMED, "Refresh manager"));
    // Create refresh algorithm instance.
    m_timed_managers.find(1)->second->createInstance(1, 1);
}