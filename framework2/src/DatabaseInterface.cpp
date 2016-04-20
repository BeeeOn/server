/* 
 * File:   DatabaseInterface.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 23. March 2016
 */

#include <iostream>

#include "DatabaseInterface.h"
#include "Session.h"

std::shared_ptr<DatabaseInterface> DatabaseInterface::m_instance;

DatabaseInterface::DatabaseInterface()
{
}

DatabaseInterface::~DatabaseInterface()
{
    std::cout << "DatabaseInterface::~DatabaseInterface" << std::endl;
}

void DatabaseInterface::createInstance()
{
    if (!m_instance) {
        std::cout << "Create DatabaseInterface:m_instance." << std::endl;
        m_instance = std::shared_ptr<DatabaseInterface>(new DatabaseInterface);
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
    std::cout << "Creating connection pool. Connecting to database." << std::endl;
    
    m_connection_pool = std::make_shared<soci::connection_pool>(sessions_count);
    
    for (size_t i = 0; i != sessions_count; i++) {
        
        soci::session &sql = m_connection_pool->at(i);
        
        sql.open(soci::postgresql, connection_string);
    }
}

std::shared_ptr<soci::session> DatabaseInterface::makeNewSession()
{ 
    return std::make_shared<soci::session>(*m_connection_pool);
}
