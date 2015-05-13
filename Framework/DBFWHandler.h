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

#ifndef DBHANDLER_H_
#define DBHANDLER_H_

class DBFWHandler
{
private:
	const std::string _Name = "DBFWHandler";
	soci::session *ses;					//Sezeni nad databazi
	std::string _DBName;					
	Loger *_log;
public:
	~DBFWHandler();
	DBFWHandler(soci::session *ses_init, Loger *loger);
	soci::session *GetConnectionSession();
	float GetValueFromDevices(std::string ID, std::string type);
	bool UpdateValueOfDevices(std::string ID, std::string type, std::string value);
	std::vector<std::string> GetNotifStringByUserId(std::string userId);
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
	std::string SelectAdapterIdByUsersAlgId(std::string UsersAlgId);
	std::vector<std::string> SelectAllIdsOfUsersAlgorithmsByAdapterId(std::string adapterId);
	std::string SelectDevTypeByDevId(std::string devId);
	std::vector<std::string> SelectDevIdsByUsersAlgId(std::string UsersAlgId);
	std::string SelectUserIdByUsersAlgId(std::string UsersAlgId);
	std::vector<std::string> SelectIdsEnabledAlgorithmsByAdapterId(std::string adapterId);
	std::string SelectStateByUsersAlgId(std::string UsersAlgId);
	std::vector<std::string> SelectAllIdsOfUsersAlgorithmsByAdapterIdAndUserId(std::string adapterId, std::string userId);
	bool InsertNotification(std::string text, std::string level, std::string timestamp, std::string userId, std::string messageId, std::string read, std::string name);
	std::string GetHighestIdInNotificationsPerUser(std::string userId);
	bool DeleteMobileDeviceByGcmId(std::string GCMId);
	std::vector<std::string> SelectIdsAlgorithmsByAlgIdAndUserId(std::string userId, std::string algId);
	bool InsertAlgList(std::string algorithm_id, std::string name, std::string type);
	bool UpdateAlgList(std::string algorithm_id, std::string name, std::string type);
};

#endif /* DBHANDLER_H_ */
