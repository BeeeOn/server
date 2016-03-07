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

//#include <chrono>
#include <iostream>
#include <string>
//#include <thread>

//#include "../../lib/pugixml.hpp"
//#include "../../lib/pugiconfig.hpp"

#include "TasksConfigParser.h"
#include "TimedTaskManager.h"

ManagerLoader::ManagerLoader() {
}

ManagerLoader::ManagerLoader(const ManagerLoader& orig) {
}

ManagerLoader::~ManagerLoader() {
}

void ManagerLoader::loadAlgorithmManagers() {
    
    // Create refresh algorithm manager.
    //m_timed_managers.emplace(1, std::make_shared<RefreshManager>(MANAGER_TYPE::TIMED, "Refresh manager"));
    // Create refresh algorithm instance.
    //m_timed_managers.find(1)->second->createInstance(1, 1);
}

void ManagerLoader::loadAllTasks(std::string tasks_config_file) {
    
    // Parse tasks configuration file.
    try {
        TasksConfigParser tasks_config_parser;
        tasks_config_info = tasks_config_parser.parseTasksConfigFile(tasks_config_file);
    }
    catch (...) {
        std::cerr << "Config file parsing was not successful." << std::endl;
        throw;
    }
    
    try {
        for (TaskConfigInfo info : tasks_config_info) {
            std::cout << "ID: " << info.id << " VERSION: " << info.version << " NAME: " << info.name << " TYPE: " << (int)info.type  << " PATH: " << info.path << std::endl;

            std::cout << "Starting to load task: " << info.name << std::cout;


            if (info.type == MANAGER_TYPE::TIMED) {
                loadTimedTask(info);
            }



            std::cout << "Successfuly finished loading task: " << info.name <<  std::cout;
        }
    }
    catch (...) {
        std::cerr << "Loading tasks was not successful." << std::endl;
        throw;
    }
}

void ManagerLoader::loadTimedTask(TaskConfigInfo info) {
    
    typedef TimedTaskManager*(*load_task_manager_fcn)(MANAGER_TYPE, std::string);

    // Open the libLowPass shared library
    void *library = dlopen(info.path.c_str(), RTLD_NOW);
    if (!library) {
        std::cout << "Failed to open library" << std::endl;
        throw;
    }
    else {
        std::cout << "dlopen() successful." << std::endl;
        loaded_task_libraries.insert({info.id, library});
        // Reset errors.
        dlerror();
    }
    
    load_task_manager_fcn loadManager = (load_task_manager_fcn)dlsym(library, "loadManager");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "dlsym(): Cannot load symbol loadManager: " << dlsym_error << std::endl;
        throw;
    }
    else {
        std::cout << "dlsym(): loadManager loaded." << std::endl;
    }
    
    
    TimedTaskManager *timed_manager = loadManager(info.type, info.name);
    
    m_timed_managers.insert({info.id, timed_manager});
    
    std::cout << "Timed manager: " << m_timed_managers.find(info.id)->second->getName() << " loaded and inserted." << std::endl;

    m_timed_managers.find(info.id)->second->createInstance(1, 1);
}
/*
void ManagerLoader::closeAllTasks() {
    
    typedef void(*delete_task_manager_fcn)(void*);
    
    for (auto library : loaded_task_libraries ) {
        dlerror();
        
        delete_task_manager_fcn deleteManager = (delete_task_manager_fcn)dlsym(library, "deleteManager");
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            std::cerr << "dlsym(): Cannot load symbol deleteManager: " << dlsym_error << '\n';
            throw;
        }
        else {
            std::cout << "dlsym(): deleteManager loaded." << std::endl;
            deleteFilter( find);
        }
    }
    
        if (dlclose(library) != 0) {
        std::cout << "couldn't close library." << std::endl;
    };
}
*/
