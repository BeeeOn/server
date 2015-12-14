/**
 * @file DBHandler.cpp
 * 
 * @brief implementation of DBHandler class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "DBHandler.h"

using namespace soci;

void DBHandler::LogValue (tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::LogValue");
	for (int i = 0; i<message->values_count;i++) //for all values create text representation and call SQL querry
	{
		try
		{
			std::string retRec;
			//std::string val = "0";
			std::string val = std::to_string(message->values[i].measured_value);
			/*switch(message->values[i].type)
			{
				//int
				case ENUM:
				case HUMIDITY:
				case PRESSURE:
				case CO2:
				case BATTERY:
				case RSSI:
				case REFRESH:
					val = std::to_string(message->values[i].ival);
					break;
				//float
				case TEMPERATURE:
				case LIGHT:
				case NOISE:
					val = std::to_string(message->values[i].fval);
					break;
				default:
					this->_log->WriteMessage(WARN,"Unknown value type nothing will be saved to DB!");
					continue;
					break;
			}
			*/
			*_sql << SQLQueries::InsertLog,
					use (message->device_euid,"DEVICE_EUID"), // device_mac (device_euid)
					use(std::to_string(message->timestamp),"MEASURED_AT"), // measured_at
					use (std::to_string(message->values[i].module_id),"MODULE_ID"), // module_id
					use (val,"MEASURED_VALUE"),
                                        use(message->adapterINTid, "GATEWAY_ID"); //measured_value
		}
		catch(std::exception const &e)
		{
			this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
			std::string ErrorMessage = "Database Error : ";
			ErrorMessage.append (e.what());
			this->_log->WriteMessage(ERR,ErrorMessage );
		}
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::LogValue");
}

DBHandler::DBHandler(soci::session *SQL, Loger *l)
{
	this->_log = l;
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	this->_sql = SQL;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}

bool DBHandler::IsInDB(std::string tableName, std::string columnName, std::string record)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::IsInDB");
	this->_log->WriteMessage(TRACE, "params : " + columnName + " " + record + " " + tableName);
	try
	{
		int retRec;
		std::string sqlQuery = "select count(*)" + columnName + " from " + tableName + " where " + columnName + " = "+ record + ";";
		this->_log->WriteMessage(TRACE,sqlQuery);
		*_sql<<sqlQuery,into(retRec);
		if (retRec > 0)
		{
			this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::IsInDB");
			return (true);
		}
		else
		{
			this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::IsInDB");
			return (false);
		}
	}
	catch(std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::IsInDB");
		return (false);
	}
}

bool DBHandler::InsertGateway(tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::InsertGateway");
	try
	{
		*_sql << SQLQueries::InsertGateway,
				use(message->adapterINTid,"GATEWAY_ID"),
				use(std::to_string(message->fm_version),"FM_VERSION"),
				use(message->socket,"SOCKET");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertGateway");
		return (true);
	}
	catch(std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertGateway");
		return (false);
	}
}

bool DBHandler::UpdateGateway(tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::UpdateGateway");
	try
	{
		std::string retRec;
		if (message->socket>0)
		{
		*_sql <<SQLQueries::UpdateGatewaySocket,
				use(message->adapterINTid,"GATEWAY_ID"),
				use(std::to_string(message->fm_version),"FM_VERSION"),
				use(message->socket,"SOCKET");
		}
		else
		{
			*_sql <<SQLQueries::UpdateGateway,
				use(message->adapterINTid,"GATEWAY_ID"),
				use(std::to_string(message->fm_version),"FM_VERSION");
		}
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::UpdateGateway");
		return (true);
	}
	catch(std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::UpdateGateway");
		return (false);
	}
}

bool DBHandler::InsertSenAct(tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::InsertSenAct");
	try
	{
		std::string retRec;
		*_sql << SQLQueries::InsertDevice,
					use(message->device_type, "DEVICE_TYPE"),
					use(message->device_euid,"DEVICE_EUID"),
					use(message->timestamp,"TIMESTAMP"),
					//use(message->battery,"battery"),
					//use(message->signal_strength,"signal"),
					use(message->adapterINTid,"GATEWAY_ID");
	}
	catch (std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error while inserting sensor/actor details : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertSenAct");
		return (false);
	}
	try
	{
		for (int i=0;i<message->values_count;i++) //for all values create text representation and call SQL querry
		{
			std::string retRec;
			//std::string val = "0";
			std::string val = std::to_string(message->values[i].measured_value);
			*_sql << SQLQueries::InsertModule,
					use(message->device_euid,"DEVICE_EUID"),
					use(message->values[i].module_id,"MODULE_ID"),
					use(val,"MEASURED_VALUE"),
                                        use(message->adapterINTid,"GATEWAY_ID");
		}

		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertSenAct");
		return (true);
	}
	catch(std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error while inserting values : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertSenAct");
		return (false);
	}
}

bool DBHandler::UpdateSenAct(tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::UpdateSenAct");
	try
	{

		std::string retRec;
		statement devStl = (_sql->prepare << SQLQueries::UpdateDevice,
    			into(retRec),
    			//use(message->battery,"battery"),
    			//use(message->signal_strength, "quality"),
    			use(message->device_euid, "DEVICE_EUID"),
    			use(message->timestamp, "MEASURED_AT"),
    			use(message->adapterINTid,"GATEWAY_ID")
          );
    devStl.execute(false);
    
    if (devStl.get_affected_rows()<=0)
		{
  		*_sql << SQLQueries::InsertDevice,
  					use(message->device_type, "DEVICE_TYPE"),
  					use(message->device_euid,"DEVICE_EUID"),
  					use(message->timestamp,"TIMESTAMP"),
  					//use(message->battery,"battery"),
  					//use(message->signal_strength,"signal"),
  					use(message->adapterINTid,"GATEWAY_ID");
		}      
          
		for (int i = 0;i<message->values_count;i++)  //for all values create text representation and call SQL querry
		{
			//std::string val = "0";
			std::string val = std::to_string(message->values[i].measured_value);
			
			statement stl = (_sql->prepare <<SQLQueries::UpdateModule,
								use(message->device_euid,"DEVICE_EUID"),
								use(val,"MEASURED_VALUE"),
								use(message->values[i].module_id,"MODULE_ID"),
                                                                use(message->adapterINTid, "GATEWAY_ID"));
			stl.execute(false);
			if (stl.get_affected_rows()<=0)
			{
				this->_log->WriteMessage(INFO,"Update affected 0 rows going to insert");
//                                std::cout << "modID: " << message->values[i].module_id << std::endl;
				*_sql << SQLQueries::InsertModule,
						use(message->device_euid,"DEVICE_EUID"),
						use(message->values[i].module_id,"MODULE_ID"),
						use(val,"MEASURED_VALUE"),
                                                use(message->adapterINTid, "GATEWAY_ID");
			}
		}
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::UpdateSenAct");
		return (true);
	}
	catch(std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::UpdateSenAct");
		return (false);
	}
}

int DBHandler::GetWakeUpTime(std::string record, long long int gateway_id)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetWakeUpTime");
	try
	{
		indicator ind;
		int retRec;
		*_sql << SQLQueries::SelectTime,
				into(retRec, ind),
				use(record),
                                use(gateway_id);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetWakeUpTime");
		return (retRec);

	}
	catch(std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetWakeUpTime");
		return (5);
	}
}

void DBHandler::GetAdapterData(int *soc, long int ID)
{
	indicator ind;
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetAdapterData");
	try
	{
		*_sql<<SQLQueries::SelectSocket,
				into(*soc,ind),
				use(ID);
		if (ind!=i_ok)
		{
			*soc=-1;
		}
	}
	catch(std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		*soc = -1;
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetAdapterData");
}

float DBHandler::GetLastTemp(std::string ID, std::string type)
{
	double retVal;
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetLastTemp");
	//std::string sqlQuery = "SELECT value FROM devices Where (fk_facilities_mac='" + ID + "' AND type =" + type +");" ;
	std::string sqlQuery = "SELECT measured_value FROM module WHERE (device_euid='" + ID + "' AND module_id =" + type +");" ;
	this->_log->WriteMessage(TRACE,sqlQuery);
	try
	{
		*_sql<<sqlQuery, into(retVal);
	}
	catch(std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		retVal = 0.0;
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetLastTemp");
	return (retVal);
}

std::vector<std::string> *DBHandler::GetEmails(std::string AdapterID)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetEmails");
	//std::string sqlQuery = "SELECT mail FROM users INNER JOIN users_adapters ON user_id=fk_user_id WHERE (fk_adapter_id=" + AdapterID + ");" ;
	std::string sqlQuery = "SELECT mail FROM users LEFT JOIN users_gateway USING(user_id) WHERE (gateway_id=" + AdapterID + ");" ;
	this->_log->WriteMessage(TRACE,sqlQuery);
	std::vector<std::string> * retVal = new std::vector<std::string>(10);
	try
	{
		*_sql<<sqlQuery, into(*retVal) ;
	}
	catch(std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		delete (retVal);
		retVal = nullptr;
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetEmails");
	return (retVal);
}

std::vector<std::string> *DBHandler::GetNotifString(std::string email)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetNotifString");
	std::string sqlQuery = "SELECT push_notification FROM users INNER JOIN mobile_devices ON user_id=fk_user_id WHERE (mail='" + email + "');" ;
	//std::string sqlQuery = "SELECT service_reference_id FROM push_notification_service WHERE user_id=fk_user_id WHERE (mail='" + email + "');" ;
	this->_log->WriteMessage(TRACE,sqlQuery);
	std::vector<std::string> * retVal = new std::vector<std::string>(10);
	try
	{
		*_sql<<sqlQuery, into(*retVal) ;
	}
	catch(std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		delete (retVal);
		retVal = nullptr;
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetNotifString");
	return (retVal);
}

DBHandler::~DBHandler()
{
}

bool DBHandler::DeleteFacility(std::string ID, long long int gateway_id)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::DeleteFacility");
	try
	{
		*_sql << SQLQueries::DeleteDevice,
				use(ID,"DEVICE_EUID"),
                                use(gateway_id, "GATEWAY_ID");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::DeleteFacility");
		return (true);
	}
	catch(std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::DeleteFacility");
		return (false);
	}
}

time_t DBHandler::GetLastTimestamp(long long unsigned int dev_euid)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetLastTimestamp");
	time_t retVal = -1;
	try
	{
		*_sql << SQLQueries::SelectTimestamp,
				into(retVal),
				use(dev_euid,"DEVICE_EUID");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetLastTimestamp");
		return (retVal);
	}
	catch(std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetLastTimestamp");
		return (retVal);
	}
}