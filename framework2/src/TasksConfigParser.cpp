/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TasksConfigParser.cpp
 * Author: mrmaidx
 * 
 * Created on 7. března 2016, 20:33
 */

#include "TasksConfigParser.h"


TasksConfigParser::TasksConfigParser() {
}

TasksConfigParser::TasksConfigParser(const TasksConfigParser& orig) {
}

TasksConfigParser::~TasksConfigParser() {
}

std::vector<TaskConfigInfo> TasksConfigParser::parseTasksConfigFile(std::string tasks_config_file) {

    std::vector<TaskConfigInfo> tasks_config_info;
    
    pugi::xml_document config_doc;
    
    pugi::xml_parse_result result = config_doc.load_file(tasks_config_file.c_str());
    
    if (!result) {
        std::cerr << "Could not open tasks config file: " << tasks_config_file << std::endl;
        std::cout << "Error description: " << result.description() << std::endl;
        std::cout << "Error offset: " << result.offset << " (error at [..." << (tasks_config_file.c_str() + result.offset) << "]" << std::endl;
        throw;
    }
    
    pugi::xml_node tasks = config_doc.child("tasks");


    for (pugi::xml_node task = tasks.child("task"); task; task = task.next_sibling("task"))
    { 
        TaskConfigInfo task_info;
        task_info.id = task.attribute("id").as_int();
        task_info.version = task.attribute("version").as_int();
        task_info.name = task.child_value("name");
        
        std::string type = task.child_value("type");
        if (type == "timed") {
            task_info.type = MANAGER_TYPE::TIMED;
        }
        else if (type == "trigger") {
            task_info.type = MANAGER_TYPE::TRIGGER;
        }    
        else if (type == "combined") {
            task_info.type = MANAGER_TYPE::COMBINED;
        }
        else {
            std::cerr << "Type must be timed, trigger or combined: " << task_info.name << std::endl;
            throw;
        }

        task_info.path = task.child_value("path");
        
        tasks_config_info.push_back(task_info);
    }
    
    return tasks_config_info;
}
