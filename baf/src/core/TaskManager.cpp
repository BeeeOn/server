/*
 * File:   TaskManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#include "TaskManager.h"

#include <sstream>
#include <stdexcept>

#include "server/DatabaseInterface.h"
#include "Logger.h"
#include "core/TaskLoader.h"
#include "model/TaskInstance.h"
#include "core/Calendar.h"

TaskManager::TaskManager()
{
}

TaskManager::~TaskManager()
{
}

long TaskManager::createInstance(CreateMessage create_message)
{
	long instance_id;

	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();

	*sql << "INSERT INTO instance(user_id, task_id) VALUES (:user_id, :task_id) RETURNING instance_id",
			soci::into(instance_id),
			soci::use(create_message.user_id, "user_id"),
			soci::use(create_message.task_id, "task_id");

	return instance_id;
}

void TaskManager::deleteInstance(DeleteMessage delete_message)
{
	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();

	*sql << "DELETE FROM instance WHERE instance_id = :instance_id",
			soci::use(delete_message.instance_id, "instance_id");

	std::lock_guard<std::mutex> lock(m_task_instances_mx);

	// Remove all entries of instance from its control component (Calendar or DataMessageRegister).
	auto instance_it = m_task_instances.find(delete_message.instance_id);
	if (instance_it != m_task_instances.end()) {
		instance_it->second->deleteFromControlComponent();

		m_task_instances.erase(delete_message.instance_id);
	}
	else {
		throw std::runtime_error(std::string("Instance with ID: ") + std::to_string(delete_message.instance_id)
			  + std::string(" doesn't exist in system. It could not be deleted."));
	}
}

std::vector<long> TaskManager::getInstanceIds(GetInstIdsMessage get_inst_ids_message)
{
	std::vector<long> instance_ids;

	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();

	// Using rowset, because we don't know how many rows will be returned.
	soci::rowset<soci::row> rows = (sql->prepare << "SELECT instance_id FROM instance "
									"WHERE task_id = :task_id AND user_id = :user_id",
									soci::use(get_inst_ids_message.task_id, "task_id"),
									soci::use(get_inst_ids_message.user_id, "user_id"));

	for (soci::rowset<soci::row>::const_iterator it = rows.begin(); it != rows.end(); ++it) {

		soci::row const& row = *it;
		// For some reason, get<> doesn't support long.
		// On 64bit Linux it shouldn't be a problem,
		// because int is same size as serial in DB.
		instance_ids.push_back(row.get<int>(0));
	}
	return instance_ids;
}


void TaskManager::suicideInstance(long instance_id)
{
	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();

	*sql << "DELETE FROM instance WHERE instance_id = :instance_id",
			soci::use(instance_id, "instance_id");

	std::lock_guard<std::mutex> lock(m_task_instances_mx);

	// Remove all entries of instance from its control component (Calendar or DataMessageRegister).
	auto instance_it = m_task_instances.find(instance_id);
	if (instance_it != m_task_instances.end()) {

		instance_it->second->deleteFromControlComponent();

		m_task_instances.erase(instance_id);
	}
	else {
		throw std::runtime_error(std::string("Instance with ID: ") + std::to_string(instance_id)
			  + std::string(" doesn't exist in system. It could not be deleted."));
	}
}

void TaskManager::deleteAllInstances()
{
	std::lock_guard<std::mutex> lock(m_task_instances_mx);
	m_task_instances.clear();
}
