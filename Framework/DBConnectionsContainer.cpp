/**
* @file databaseConnectionContainer.cpp
*
* Implementace kontejneru pro praci s databazi pomoci knihovny soci
*
*/

#include "DBConnectionsContainer.h"


using namespace soci;
using namespace std;

DBConnectionsContainer *DBConnectionsContainer::instance = NULL;

/**
* Konstruktor kontejneru pro práci s databazi. Je vyuzit pouze interne
*
* @param NameOfDB    nazev db ke ktere se pripojuje
* @param ConnLimit   pocet reprezentujici omezeni pripojeni k databazi
* @param l			 instance pro nastaveni logovani v tride  DatabaseConnectionContainer
*/
DBConnectionsContainer::DBConnectionsContainer(std::string NameOfDB, int ConnLimit, Loger *l)
{
	this->_log = l;
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::DBConnectionsContainer");
	_NameOfDB = NameOfDB;
	_log->WriteMessage(INFO,"Creating connections to DB");
	int realFree = 0;
	for (int i = 0; i < ConnLimit; i++)
	{
		try
		{
			session *SQL = new session(postgresql, "dbname=" + this->_NameOfDB);
			this->connections[i] = SQL;
            realFree++;
		}
		catch(std::exception const &e)
		{
			std::string ErrorMessage = "Creating DB connection number : " + std::to_string(i) + "failed reason :";
			ErrorMessage.append (e.what());
			this->_log->WriteMessage(ERR,ErrorMessage );
		}
	}
	this->freeCount = realFree;
	this->_log->WriteMessage(INFO,"Connections to DB created");
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::DBConnectionsContainer");
}
/**
* Destruktor kontejneru pro praci s databazi
*
*/
DBConnectionsContainer::~DBConnectionsContainer()
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
/**
* Navrati spojeni zpet do kontejneru pomoci session (knihovna soci)
*
* @param conn	spojeni s databazi
*/
void DBConnectionsContainer::ReturnConnection(session *conn)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::ReturnConnection");
	this->semaphore.lock();
	this->connections[this->freeCount] = conn;
	this->freeCount++;
	this->semaphore.unlock();
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReturnConnection");
}

/**
* Vypise poèet volných spojení v kontejneru.
*
*/
int DBConnectionsContainer::Limit()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Limit");
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Limit");
	return freeCount;
}

/**
* Vyda spojeni z kontejneru, se kterym je pak mozne pracovat.
*
* @return session Session je tedy spojeni s databazi, se kterym je mozno pracovat.
*/
session *DBConnectionsContainer::GetConnection()
{
	this->_log->WriteMessage(TRACE,"Entering DBConnectionsContainer::GetConnection");
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
	this->_log->WriteMessage(TRACE,"Exiting DBConnectionsContainer::GetConnection");
	return retConn;
}

/**
* Vytvori kontejner - vyuziva navrhoveho vzoru jedinacek. Parametry jsou stejne jako v samotnem konstruktoru.
*
* @param NameOfDB    nazev db ke ktere se pripojuje
* @param ConnLimit   pocet reprezentujici omezeni pripojeni k databazi
* @param l			 instance pro nastaveni logovani v tride  DatabaseConnectionContaineru
*/
DBConnectionsContainer *DBConnectionsContainer::GetConnectionContainer(Loger *l, std::string NameOfDB, int ConnLimit)
{
	l->WriteMessage(TRACE,"Entering  DBConnectionsContainer::CreateContainer");
	if (!instance)
		instance = new DBConnectionsContainer(NameOfDB, ConnLimit, l);
	l->WriteMessage(TRACE,"Exiting  DBConnectionsContainer::CreateContainer");
	return instance;
}
