/*
 * DBHandler.h
 *
 *  Created on: Oct 26, 2014
 *      Author: tuso
 */

#include <soci.h>
#include <postgresql/soci-postgresql.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include "structures.h"
#include "loger.h"
#include "sqlCommands.h"

#ifndef DBHANDLER_H_
#define DBHANDLER_H_


class DBHandler
{
	private:
		const std::string _Name="DBHandler";
		soci::session *_sql;  /**< ukazatel na pripojenie k databaze*/
		std::string _DBName;  /**< nazov databazy*/
		Loger *_log;
	public:
		~DBHandler();
		DBHandler(soci::session *SQL, Loger *l);
		bool DeleteFacility(std::string ID);
		bool IsInDB(std::string tableName,std::string columnName,std::string record);
		bool InsertAdapter(tmessage *message);
		bool InsertSenAct(tmessage *message);
		bool UpdateAdapter(tmessage *message);
		bool UpdateSenAct(tmessage *message);
		int GetWakeUpTime(std::string record);
		void LogValue (tmessage *message);
		void GetAdapterData(int *soc, long int ID);
		bool UpdateAdapterPort(tmessage *message);
		float GetLastTemp(std::string ID, std::string type);
		std::vector<std::string> *GetEmails(std::string AdapterID);
		std::vector<std::string> *GetNotifString(std::string email);
};


#endif /* DBHANDLER_H_ */
