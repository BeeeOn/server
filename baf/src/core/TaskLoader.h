/*
 * File:   TaskLoader.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef TASKLOADER_H
#define TASKLOADER_H

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "pugixml.hpp"

#include "core/BaseTaskLoader.h"
#include "model/Task.h"

struct TaskInfo {
	unsigned int task_id;
	unsigned short task_version;
	std::string task_name;
	TASK_TYPE task_type;
	std::string task_path;
};

class TaskLoader: public BaseTaskLoader
{
public:
	/**
	 * Destructor of class TaskLoader.
	 */
	virtual ~TaskLoader();
	/**
	 * Creates singleton instance of class DatabaseInterface.
	 * Can be called only once at start of BAF.
	 */
	static void createInstance();
	/**
	 * Loads and runs all tasks defined in tasks config file.
	 * @param tasks_config_file_path Path to tasks config file.
	 */
	void createAllTasks(std::string tasks_config_file_path) override;
	/**
	 * Loads tasks which IDs are new in tasks config file.
	 */
	void createNewTasks() override;
	/**
	 * Processes config file and return informations about all tasks.
	 * @return Vector of information about tasks.
	 */
	std::vector<TaskInfo> parseAllTasks();
	/**
	 * Inserts tasks into database and system and reloads instances.
	 */
	void createParsedTasks(std::vector<TaskInfo> tasks_info);
	/**
	 * Closes all tasks in TaskLoader.
	 */
	void closeAllTasks();

protected:
	/**
	 * Private constructor of class TaskLoader.
	 */
	TaskLoader();
	/**
	 * Delete copy constructor.
	 */
	TaskLoader(const TaskLoader& orig) = delete;
	/**
	 * Delete assignment operator.
	 */
	void operator=(const TaskLoader&) = delete;
};

#endif /* TASKLOADER_H */
