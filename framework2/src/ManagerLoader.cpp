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

std::map<int /*manager_id*/, std::shared_ptr<TimedAlgorithmManager>> ManagerLoader::m_timed_managers;

ManagerLoader::ManagerLoader() {
}

ManagerLoader::ManagerLoader(const ManagerLoader& orig) {
}

ManagerLoader::~ManagerLoader() {
}

void ManagerLoader::loadAlgorithmManagers() {
    
    // Create test timed manager.
    m_timed_managers.emplace(10, std::make_shared<RefreshManager>(10, MANAGER_TYPE::TIMED, "Refresh test"));
    // Create test instance.
    //std::cout << m_timed_managers.find(10)->second->getId() << std::endl;
    m_timed_managers.find(10)->second->createInstance(1, 1, "Refresh instance cislo 1 se spustila.");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    m_timed_managers.find(10)->second->createInstance(2, 2, "Refresh instance cislo 2 se spustila.");
    
}

void ManagerLoader::activateInstance(int manager_id, unsigned long instance_id) {
    m_timed_managers.find(manager_id)->second->activateInstance(instance_id);
}
