/* 
 * File:   TaskLoader.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TaskLoader.h"

#include <sstream>

#include "DatabaseInterface.h"
#include "Logger.h"
#include "Task.h"

TaskLoader::TaskLoader()
{
}

TaskLoader::~TaskLoader()
{
}

void TaskLoader::createInstance()
{
    if (!m_instance) {
        
        logger.LOGFILE("task_loader", "INFO") << "TaskLoader created." << std::endl;
        m_instance = std::shared_ptr<TaskLoader>(new TaskLoader);   
    }
}

void TaskLoader::createAllTasks(std::string tasks_config_file_path)
{
    // Store path to config file in member variable.
    m_tasks_config_file_path = tasks_config_file_path;
    
    std::vector<TaskInfo> tasks_info;
    try {
        // Parse config file and create task objects.
        std::vector<TaskInfo> tasks_info = parseAllTasks();
        // Create parsed tasks.
        createParsedTasks(tasks_info);
        
        logger.LOGFILE("task_loader", "INFO") << "All tasks were succesfully created. Number of tasks: "
                << tasks_info.size() << std::endl;
    }
    catch (const std::exception& e) {
        logger.LOGFILE("task_loader", "ERROR") << e.what() << std::endl;
    }
}

void TaskLoader::createNewTasks()
{
    std::vector<TaskInfo> tasks_info;
    try {
        // Parse config file and create task objects which are new.
        std::vector<TaskInfo> tasks_info = parseAllTasks();
        
        if (tasks_info.size() != 0) {
            // Create parsed tasks.
            createParsedTasks(tasks_info);
            logger.LOGFILE("task_loader", "INFO") << "New tasks were succesfully created."
                    << "Number of new tasks: " << tasks_info.size() << std::endl;
        }
        else {
            logger.LOGFILE("task_loader", "INFO") << "No new tasks in tasks config file." << std::endl;
        }
    }
    catch (const std::exception& e) {
        logger.LOGFILE("task_loader", "ERROR") << e.what();
    }
}

void TaskLoader::createParsedTasks(std::vector<TaskInfo> tasks_info)
{
    logger.LOGFILE("task_loader", "INFO") << "Processing tasks config file." << std::endl;
    
    for (auto task_info : tasks_info) {
        
        try {
            // Get type as string.
            std::string type_str;
            if (task_info.task_type == TASK_TYPE::TIMED) {
                type_str = "timed";
            }
            else if (task_info.task_type == TASK_TYPE::TRIGGER) {
                type_str = "trigger";
            }
            else {
                type_str = "combined";
            } 
            
            try {
                // Insert task information to database.
                SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
                
                short exists;
                *sql << "SELECT exists(SELECT 1 FROM task WHERE task_id = :task_id);",
                        soci::use(task_info.task_id, "task_id"),
                        soci::into(exists);
                
                if (!exists) {
                    // Insert only if it doesn't exist.
                    *sql << "INSERT INTO task(task_id, name, version, type) VALUES(:task_id, :name, :version, :type)",
                             soci::use(task_info.task_id, "task_id"),
                             soci::use(task_info.task_name, "name"),
                             soci::use(task_info.task_version, "version"),
                             soci::use(type_str, "type");

                    logger.LOGFILE("task_loader", "INFO") << "Task: " << task_info.task_name
                            << " was inserted to database." << std::endl;
                }
                else {
                    logger.LOGFILE("task_loader", "INFO") << "Task with ID: " << task_info.task_id
                            << " already is in database." << std::endl;
                }
            }
            catch (const std::exception& e) {
                logger.LOGFILE("task_loader", "ERROR") << "Task: " << task_info.task_name
                        << " storing to database was unsuccessful: " << e.what() << std::endl;
            }
            
            // Emplace task into BAF.
            m_tasks.emplace(task_info.task_id, std::make_shared<Task>(task_info.task_version, task_info.task_name,
                                                                      task_info.task_type, task_info.task_path));
            
            logger.LOGFILE("task_loader", "INFO") << "Task: " << task_info.task_name
                    << " was emplaced into BAF." << std::endl;
            
            // Get pointer to iterator to emplaced task.
            auto task_it = m_tasks.find(task_info.task_id);
            // Open task library.
            task_it->second->openTaskLibrary();
            // Create task manager.
            task_it->second->createTaskManager();
            
            logger.LOGFILE("task_loader", "INFO") << "Manager of task: " << task_info.task_name
                    << " was created." << std::endl;
            
            // Reload instances which already exists.
            task_it->second->getTaskManagerPtr()->reloadInstances(task_it->first);
            
            logger.LOGFILE("task_loader", "INFO") << "Instances of task: " << task_info.task_name
                    << " were reloaded." << std::endl;
        }
        catch (const std::exception& e) {
            logger.LOGFILE("task_loader", "ERROR") << e.what() << std::endl;
        }
    }
}

std::vector<TaskInfo> TaskLoader::parseAllTasks()
{
    logger.LOGFILE("task_loader", "INFO") << "Processing tasks config file." << std::endl;
    
    std::vector<TaskInfo> tasks_info;
    
    pugi::xml_document config_doc;
    
    pugi::xml_parse_result result = config_doc.load_file(m_tasks_config_file_path.c_str());
    
    if (!result) {
        logger.LOGFILE("config_parser", "FATAL") << "Could not open baf config file: "
                << m_tasks_config_file_path << ". Error description: " << result.description()
                << ", Error offset: " << result.offset << " (error at [..."
                << (m_tasks_config_file_path.c_str() + result.offset) << "]" << std::endl;
        
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    
    pugi::xml_node tasks = config_doc.child("tasks");
    
    for (pugi::xml_node task = tasks.child("task"); task; task = task.next_sibling("task")) {
        
        try {
            TaskInfo task_info;

            task_info.task_id = task.attribute("id").as_int();
            task_info.task_version = task.attribute("version").as_int();
            task_info.task_name = task.child_value("name");

            std::string type_str = task.child_value("type");
            if (type_str == "timed") {
                task_info.task_type = TASK_TYPE::TIMED;
            }
            else if (type_str == "trigger") {
                task_info.task_type = TASK_TYPE::TRIGGER;
            }    
            else if (type_str == "combined") {
                task_info.task_type = TASK_TYPE::COMBINED;
            }
            else {
                std::stringstream error;
                error << "Type must be timed, trigger or combined: " << task_info.task_name << std::endl;
                throw std::runtime_error(error.str());
            }
            task_info.task_path = task.child_value("path");
            
            // Check if task is not already in BAF.
            if (m_tasks.find(task_info.task_id) != m_tasks.end()) {
                continue;
            }
            else {
                tasks_info.push_back(task_info);
            }
        }
        catch (const std::exception& e) {
            logger.LOGFILE("task_loader", "ERROR") <<  e.what() << std::endl;
        }
    }
    return tasks_info;
}

void TaskLoader::closeAllTasks()
{
    for (auto task_ptr : m_tasks) {
        
        if (task_ptr.second->getTaskManagerPtr()) {
            
            task_ptr.second->getTaskManagerPtr()->deleteAllInstances();
        }
        else {
        }
    }
}
