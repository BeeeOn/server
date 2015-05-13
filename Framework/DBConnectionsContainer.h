/**
* @file databaseConnectionContainer.h
*
* Hlavickovy soubor pro kontejner pro praci s databazi (pomoci knihovny soci)
*
*/

#ifndef DATABASECONNECTIONCONTAINER_H_
#define DATABASECONNECTIONCONTAINER_H_

#include "loger.h"
#include <mutex>
#include <soci.h>
#include <postgresql/soci-postgresql.h>

class DBConnectionsContainer
{
	private:
		static DBConnectionsContainer *instance;
		Loger *_log;
		const std::string _Name="DBConnectionsContainer";
		int freeCount;
		soci::session *connections[100];
		std::mutex semaphore;
		std::string _NameOfDB;
		DBConnectionsContainer(std::string DBName, int ConnLimit, Loger *l); //pouziti navrhoveho vzoru jedinacek
	public:
		static DBConnectionsContainer *GetConnectionContainer(Loger *l, std::string DBName, int ConnLimit);
		~DBConnectionsContainer();
		soci::session *GetConnection();
		void ReturnConnection(soci::session *conn);
		int Limit();
};
#endif /* DATABASECONNECTIONCONTAINER_H_ */