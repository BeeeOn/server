/* 
 * File:   ManagerLoader.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include <iostream>
#include "ManagerLoader.h"

// Algorithm includes.
#include "../algorithms/RefreshManager.h"
#include "../algorithms/WatchdogManager.h"
#include "Logger.h"

ManagerLoader::ManagerLoader() {
}

ManagerLoader::ManagerLoader(const ManagerLoader& orig) {
}

ManagerLoader::~ManagerLoader() {
}

void ManagerLoader::loadAlgorithmManagers() {
    /*
    std::cout << "watchdog load" << std::endl;
    event_managers.emplace(1, WatchdogManager(1, MANAGER_TYPE::EVENT ,"Watchdog"));
    
    std::cout << "refresh load" << std::endl;
    timed_managers.emplace(2, RefreshManager(2, MANAGER_TYPE::TIMED ,"Refresh"));
    */
    /*
    auto search = m_managers.find(1);
    if(search != algorithm_managers.end()) {
        std::cout << "Found " << search->first << " " << search->second.getName() << std::endl;
    }
    
    auto search2 = algorithm_managers.find(2);
    if(search2 != algorithm_managers.end()) {
        std::cout << "Found " << search2->first << std::endl;
    }
    */
}
