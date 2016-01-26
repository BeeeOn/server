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
    
    // Create test timed manager.
    m_timed_managers.emplace(1, std::make_shared<RefreshManager>(MANAGER_TYPE::TIMED, "Refresh manager"));
    m_timed_managers.emplace(2, std::make_shared<TestManager>(MANAGER_TYPE::TIMED, "Test manager"));
    // Create test instance.
    //std::cout << m_timed_managers.find(10)->second->getId() << std::endl;
    m_timed_managers.find(1)->second->createInstance(1, 1);
    m_timed_managers.find(2)->second->createInstance(1, 1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    m_timed_managers.find(1)->second->createInstance(2, 1);
    m_timed_managers.find(2)->second->createInstance(2, 1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    m_timed_managers.find(1)->second->createInstance(3, 1);
    m_timed_managers.find(2)->second->createInstance(3, 1);
    
    
   
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    //m_timed_managers.find(10)->second->createInstance(2, 2);
    //m_timed_managers.find(10)->second->createInstance(3, 3);
    
}