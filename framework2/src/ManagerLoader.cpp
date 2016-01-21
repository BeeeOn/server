/* 
 * File:   ManagerLoader.cpp
 * Author: mrmaidx
 * 
 * Created on 20. ledna 2016, 19:22
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

void ManagerLoader::loadAlgorithms() {
    
    std::cout << "watchdog load" << std::endl;
    algorithm_managers.emplace(1, WatchdogManager());
    std::cout << "refresh load" << std::endl;
    algorithm_managers.emplace(2, RefreshManager());
    
    auto search = algorithm_managers.find(1);
    if(search != algorithm_managers.end()) {
        std::cout << "Found " << search->first << " " << search->second.getName() << std::endl;
    }
    auto search2 = algorithm_managers.find(2);
    if(search2 != algorithm_managers.end()) {
        std::cout << "Found " << search2->first << std::endl;
    }
    
}
