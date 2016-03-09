/* 
 * File:   TaskLoader.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TaskLoader.h"

#include "Task.h"

TaskLoader::TaskLoader() {
}

TaskLoader::~TaskLoader() {
    std::cout << "TaskLoader::~TaskLoader - entered." << std::endl;
    // Close and delete all tasks.
    //closeAllTasks();
    
    std::cout << "TaskLoader::~TaskLoader - finished." << std::endl;
}

void TaskLoader::createAllTasks(std::string tasks_config_file_path) {

    try {
        // Parse config file and create task objects.
        processTasksConfigFileAndStoreInfo(tasks_config_file_path);
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
    }
    
    for (auto task_ptr : m_tasks) {
        
        try {
            // Open task library.
            task_ptr.second->openTaskLibrary();
            // Create task manager.
            task_ptr.second->createTaskManager();
        }
        catch (const std::exception& e) {
            std::cerr << e.what();
        }
    }
}

void TaskLoader::processTasksConfigFileAndStoreInfo(std::string tasks_config_file_path) {
    
    unsigned int task_id;
    unsigned short task_version;
    std::string task_name;
    TASK_TYPE task_type;
    std::string task_path;
    
    pugi::xml_document config_doc;
    
    pugi::xml_parse_result result = config_doc.load_file(tasks_config_file_path.c_str());
    
    if (!result) {
        std::cerr << "Could not open tasks config file: " << tasks_config_file_path << std::endl;
        std::cerr << "Error description: " << result.description() << std::endl;
        std::cerr << "Error offset: " << result.offset << " (error at [..." << (tasks_config_file_path.c_str() + result.offset) << "]" << std::endl;
        
        throw std::runtime_error("Parsing of tasks was not successful.");
    }
    
    pugi::xml_node tasks = config_doc.child("tasks");

    for (pugi::xml_node task = tasks.child("task"); task; task = task.next_sibling("task"))
    { 
        
        task_id = task.attribute("id").as_int();
        task_version = task.attribute("version").as_int();
        task_name = task.child_value("name");
        
        std::string type_str = task.child_value("type");
        if (type_str == "timed") {
            task_type = TASK_TYPE::TIMED;
        }
        else if (type_str == "trigger") {
            task_type = TASK_TYPE::TRIGGER;
        }    
        else if (type_str == "combined") {
            task_type = TASK_TYPE::COMBINED;
        }
        else {
            std::stringstream error;
            error << "Type must be timed, trigger or combined: " << task_name << std::endl;
            throw std::runtime_error(error.str());
        }

        task_path = task.child_value("path");
        
        std::cout << "-----TASK----- " << std::endl;
        std::cout << "ID: " << task_id << std::endl;
        std::cout << "VERSION: " << task_version << std::endl;
        std::cout << "NAME: " << task_name << std::endl;
        std::cout << "TYPE: " << (int)task_type << std::endl;
        std::cout << "PATH: " << task_path << std::endl;
        
        
        m_tasks.emplace(task_id, std::make_shared<Task>(task_version, task_name, task_type, task_path));
        
    }
}