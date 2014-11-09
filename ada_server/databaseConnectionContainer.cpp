#include "databaseConnectionContainer.h"

/**********************************************Start of DatabaseConnectionContainer section******************************************************/

using namespace soci;

DatabaseConnectionContainer::DatabaseConnectionContainer(std::string DBName, int ConnLimit, Loger *l)
{
	this->_log = l;
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::DatabaseConnectionContainer");
	_DBName = DBName;
	_log->WriteMessage(INFO,"Creating connections to DB");
	for (int i = 0; i < ConnLimit; i++)
	{
		try  //pokusime  sa o vytvorenie spojenia s databazou
		{
			session *SQL = new session(postgresql, "dbname="+this->_DBName);
			this->connections[i] = SQL;
			this->freeCount = i+1;
		}
		catch(std::exception const &e) //ak nastala chyba zachytime ju
		{
			std::string ErrorMessage = "Creating DB connection number : " + std::to_string(i) + "failed reason :";
			ErrorMessage.append (e.what());
			this->_log->WriteMessage(ERR,ErrorMessage );
		}
	}
	this->_log->WriteMessage(INFO,"Connections to DB created");
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::DatabaseConnectionContainer");
}

DatabaseConnectionContainer::~DatabaseConnectionContainer()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	this->_log->WriteMessage(INFO,"Closing connections to DB");
	for (int i = 0; i< this->freeCount; i++)
	{
		this->semaphore.lock();
		delete(connections[i]);
		this->_log->WriteMessage(INFO,"Deleted connection");
		this->semaphore.unlock();
	}
	this->_log->WriteMessage(INFO,"Connections to DB closed");
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}

void DatabaseConnectionContainer::ReturnConnection(session *conn)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::ReturnConnection");
	this->semaphore.lock();
	this->connections[this->freeCount] = conn;
	this->freeCount++;
	this->semaphore.unlock();
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReturnConnection");
}

int DatabaseConnectionContainer::Limit()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Limit");
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Limit");
	return freeCount;
}

session *DatabaseConnectionContainer::GetConnection()
{
	this->_log->WriteMessage(TRACE,"Entering  DatabaseConnectionContainer::GetConnection");
	session *retConn = NULL;
	this->semaphore.lock();
	if (this->freeCount>0)
	{
		retConn = this->connections[this->freeCount-1];
		this->connections[this->freeCount-1] = NULL;
		this->freeCount--;
		this->_log->WriteMessage(INFO,"Free connections : " + std::to_string(this->freeCount));
	}
	this->semaphore.unlock();
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetConnection");
	return retConn;
}

DatabaseConnectionContainer *DatabaseConnectionContainer::instance = NULL;

DatabaseConnectionContainer *DatabaseConnectionContainer::CreateContainer(Loger *l, std::string DBName, int ConnLimit)
{
	l->WriteMessage(TRACE,"Entering  DatabaseConnectionContainer::CreateContainer");
	if (!instance)
		instance=new DatabaseConnectionContainer(DBName, ConnLimit, l);
	l->WriteMessage(TRACE,"Exiting  DatabaseConnectionContainer::CreateContainer");
	return instance;

}

	/**********************************************End of DatabaseConnectionContainer section******************************************************/
