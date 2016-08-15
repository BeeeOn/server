/*
 * File:   DatabaseInterface.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 23. March 2016
 */

#include "server/DatabaseInterface.h"

#include "Logger.h"

// Definition of singleton instance.
std::shared_ptr<DatabaseInterface> DatabaseInterface::m_instance;

DatabaseInterface::DatabaseInterface()
{
}

DatabaseInterface::~DatabaseInterface()
{
}

void DatabaseInterface::createInstance()
{
	if (!m_instance) {
		m_instance = std::shared_ptr<DatabaseInterface>(new DatabaseInterface);
		logger.LOGFILE("database_interface", "INFO") << "DatabaseInterface created." << std::endl;
	}
}

std::shared_ptr<DatabaseInterface> DatabaseInterface::getInstance()
{
	if (m_instance) {
		return m_instance;
	}
	else {
		throw std::runtime_error("DatabaseInterface singleton was not created or already destructed.");
	}
}

void DatabaseInterface::connectToDatabase(int sessions_count, std::string connection_string)
{
	logger.LOGFILE("database_interface", "INFO") << "Creating connection pool."
			<< "Connecting to database." << std::endl;

	m_connection_pool = std::make_shared<soci::connection_pool>(sessions_count);

	for (size_t i = 0; i != sessions_count; i++) {

		soci::session &sql = m_connection_pool->at(i);

		sql.open(soci::postgresql, connection_string);
	}
}

SessionSharedPtr DatabaseInterface::makeNewSession()
{
	return std::make_shared<soci::session>(*m_connection_pool);
}
