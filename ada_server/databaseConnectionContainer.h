/*
 * databaseConnectionContainer.h
 *
 *  Created on: Oct 25, 2014
 *      Author: tuso
 */

#ifndef DATABASECONNECTIONCONTAINER_H_
#define DATABASECONNECTIONCONTAINER_H_

#include "loger.h"
#include <mutex>
#include <soci.h>
#include <postgresql/soci-postgresql.h>


class DatabaseConnectionContainer
{
	private:
		Loger *_log;
		const std::string _Name="DatabaseConnectionContainer";
		int freeCount;
		soci::session *connections[100];
		std::mutex semaphore;
		std::string _DBName;
		DatabaseConnectionContainer(std::string DBName, int ConnLimit, Loger *l);
		static DatabaseConnectionContainer *instance;
	public:
		int Limit();
		static DatabaseConnectionContainer *CreateContainer(Loger *l, std::string DBName, int ConnLimit);
		soci::session *GetConnection();
		void ReturnConnection(soci::session *conn);
		~DatabaseConnectionContainer();
};



#endif /* DATABASECONNECTIONCONTAINER_H_ */
