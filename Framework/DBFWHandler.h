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
#include "../loger.h"

#ifndef DBHANDLER_H_
#define DBHANDLER_H_


class DBHandler
{
private:
	const std::string _Name = "DBHandler";
	soci::session *_sql;  /**< ukazatel na pripojenie k databaze*/
	std::string _DBName;  /**< nazov databazy*/
	Loger *_log;
public:
	~DBHandler();
	DBHandler(soci::session *SQL, Loger *l);
	soci::session *ReturnConnection();
	bool IsInDB(std::string tableName, std::string columnName, std::string record);
	bool InsertAdapter(tmessage *message);
	bool InsertSenAct(tmessage *message);
	bool UpdateAdapter(tmessage *message);
	bool UpdateSenAct(tmessage *message);
	int GetWakeUpTime(std::string record);
	void LogValue(tmessage *message);
	void GetAdapterData(std::string *adapterIP, long int ID);
	float GetLastTemp(std::string ID, std::string type);
	std::vector<std::string> *GetEmails(std::string AdapterID);
	std::vector<std::string> *GetNotifStringByUserId(std::string userId);
	std::string GetUserIdByEmail(std::string Email);
	std::string GetEmailByUserId(std::string UserID);
	std::string InsertUserAlgorithm(std::string UserId, std::string AlgId, std::string parameters, std::string name, std::string adapterId);
	bool UpdateUserAlgorithm(std::string UserId, std::string AlgId, std::string parameters, std::string name, std::string state);
	bool InsertAlgoDevices(std::string UsersAlgorithmsId, std::string UserId, std::string AlgorithId, std::string mac, std::string type);
	bool DeleteAlgoDevices(std::string UsersAlgorithmsId);
	bool DeleteUsersAlgorithms(std::string UsersAlgorithmsId);
	std::string SelectAlgIdByUsersAlgId(std::string UsersAlgId);
	std::string SelectNameByUsersAlgId(std::string UsersAlgId);
	std::string SelectParametersByUsersAlgId(std::string UsersAlgId);
	std::vector<std::string> SelectAllIdsOfUsersAlgorithmsByAdapterId(std::string adapterId);
	std::string SelectDevTypeByDevId(std::string devId);
	std::vector<std::string> SelectDevIdsByUsersAlgId(std::string UsersAlgId);
	std::string SelectUserIdByUsersAlgId(std::string UsersAlgId);
	std::vector<std::string> SelectIdsEnabledAlgorithmsByAdapterId(std::string adapterId);
	std::string SelectStateByUsersAlgId(std::string UsersAlgId);
	std::vector<std::string> SelectAllIdsOfUsersAlgorithmsByAdapterIdAndUserId(std::string adapterId, std::string userId);
};


#endif /* DBHANDLER_H_ */
