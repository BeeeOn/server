/*
 * File:   BaseTaskLoader.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 12. April 2016
 */

#include "core/BaseTaskLoader.h"

#include <stdexcept>

#include "Logger.h"
// Definition of singleton instance.
std::shared_ptr<BaseTaskLoader> BaseTaskLoader::m_instance;

BaseTaskLoader::BaseTaskLoader()
{
}

BaseTaskLoader::~BaseTaskLoader()
{
}

std::shared_ptr<Task> BaseTaskLoader::findTask(unsigned int task_id)
{
	auto found = m_tasks.find(task_id);

	if (found != m_tasks.end()) {

		return found->second;
	}
	else {
		// Task was not found.
		std::stringstream error;
		error << "Task with id: " << task_id << " was not found in the BAF system.";
		throw std::runtime_error(error.str());
	}
}

std::shared_ptr<BaseTaskLoader> BaseTaskLoader::getInstance()
{
	if (m_instance) {
		return m_instance;
	}
	else {
		throw std::runtime_error("BaseTaskLoader singleton was not created or already destructed.");
	}
}
