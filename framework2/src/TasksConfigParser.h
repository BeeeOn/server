/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TasksConfigParser.h
 * Author: mrmaidx
 *
 * Created on 7. března 2016, 20:33
 */

#ifndef TASKSCONFIGPARSER_H
#define TASKSCONFIGPARSER_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../lib/pugixml.hpp"
#include "../../lib/pugiconfig.hpp"

#include "TaskManager.h"

struct TaskConfigInfo {
    int id;
    int version;
    std::string name;
    MANAGER_TYPE type; // MANAGER_TYPE stores a type of task.
    std::string path;
};

class TasksConfigParser {
public:
    TasksConfigParser();
    
    TasksConfigParser(const TasksConfigParser& orig);
    
    virtual ~TasksConfigParser();
    
    std::vector<TaskConfigInfo> parseTasksConfigFile(std::string tasks_config_file);

private:
    
};

#endif /* TASKSCONFIGPARSER_H */

