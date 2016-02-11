/**
 * @file DBHandler.h
 * 
 * @brief definition of DBHandler class
 *
 * @author Matus Blaho 
 * @version 1.0
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


/** @class DBHandler
 *  @brief Class responsible for database interface
 */

class DBHandler
{
	private:
		const std::string _Name="DBHandler"; /**< name of class*/
		soci::session *_sql;  /**< conncetion to dabase*/
		std::string _DBName;  /**< name of database*/
		Loger *_log; /**<reference to loger used for logging */
	public:
		~DBHandler();
		DBHandler(soci::session *SQL, Loger *l);
		bool DeleteFacility(std::string ID, long long int gateway_id);
		bool IsInDB(std::string tableName,std::string columnName,std::string record);
		/** Method for creating new adapter in database
		 * @param message - pointer to message content
		 * @return bool on success/failure true/false
		    */
		bool InsertGateway(tmessage *message);
		/** Method for inserting new facility and device to database
		 * @param message - pointer to message content
		 * @return bool on success/failure true/false
			*/
		bool InsertSenAct(tmessage *message);
		/** Method for updating adapter in database
		 * @param message - pointer to message content
		 * @return bool on success/failure true/false
			*/
		bool UpdateGateway(tmessage *message);
		/** Method for updating facility and device in database
		 * @param message - pointer to message content
		 * @return bool on success/failure true/false
			*/
		bool UpdateSenAct(tmessage *message);
		/** Method for obtaining wakeup time from database
		 * @param record - record which time are we looking for
		 * @return number of seconds to next wakeup on failure/missing record returns default value 5
			*/
		int GetWakeUpTime(std::string record, long long int gateway_id);
		std::string GetXmlDeviceParameters(std::string device_euid, long long int adapterid);
    /** Method for creating history row in database
		 * @param message - pointer to message content
			*/
		void LogValue (tmessage *message);
		/** Method for obtaining socket number from DB
		 * @param soc - pointer to int where is returned value
		 * @param long int ID of adapter
			*/
		void GetAdapterData(int *soc, long int ID);
		/** Method updating socket number in database
		 * @param message - pointer to message content
		 * @param long int ID of adapter
			*/
		bool UpdateAdapterPort(tmessage *message);
		/** Method for obtaining last temperature of device in DB - THIS METHOD IS OBSOLETE!!!
		 * @param message - pointer to message content
		 * @return bool on success/failure true/false
					*/
		float GetLastTemp(std::string ID, std::string type);
		/** Method for obtaining user emails - THIS METHOD IS OBSOLETE!!!
		 * @param std::string containing adapter which is used for looking for users
		 * @return pointer to populated vector
					*/
		std::vector<std::string> *GetEmails(std::string AdapterID);
		/** Method for obtaining user notification strings - THIS METHOD IS OBSOLETE!!!
				 * @param std::string containing email of user which is used for looking for notif string
				 * @return pointer to populated vector
							*/
		std::vector<std::string> *GetNotifString(std::string email);
		/** Method for obtaining last timestamp
		* @param long long unsigned int containing device identification
		* @return time_t value of the timestamp from database*/
		time_t GetLastTimestamp(long long unsigned int dev_euid);
};


#endif /* DBHANDLER_H_ */
