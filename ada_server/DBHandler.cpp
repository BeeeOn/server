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
			std::string val = "0";
			switch(message->values[i].type)
			{
				case TEMP:
				case LUM:
				case REZ:
				case POS:
				case BT:
					val = std::to_string(message->values[i].fval);
					break;
				case ONON:
				case TOG:
				case ONOFFSEN:
				case ONOFSW:

						if(message->values[i].bval)
							val = "1";
					
					break;
				case EMI:
				case HUM:
				case BAR:
				case RGB:
				case RAN:
				case BOT:
				case BOM:
				case BST:
					val = std::to_string(message->values[i].ival);
					break;
				default:
					this->_log->WriteMessage(WARN,"Unknown value type nothing will be saved to DB!");
					continue;
					break;
			}
			*_sql << SQLQueries::InsertLog,
					use (message->sensor_id,"deviceID"),
					use(std::to_string(message->timestamp),"timeStamp"),
					use (std::to_string(message->values[i].intType),"type"),
					use (val,"value");
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

bool DBHandler::InsertAdapter(tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::InsertAdapter");
	try
	{
		*_sql << SQLQueries::InsertAdapter,
				use(message->adapterINTid,"AdapterID"),
				use(std::to_string(message->fm_version),"FMver"),
				use(message->socket,"socket");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertAdapter");
		return (true);
	}
	catch(std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertAdapter");
		return (false);
	}
}

bool DBHandler::UpdateAdapter(tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::UpdateAdapter");
	try
	{
		std::string retRec;
		if (message->socket>0)
		{
		*_sql <<SQLQueries::UpdateAdapterSocket,
				use(message->adapterINTid,"ID"),
				use(std::to_string(message->fm_version),"FMver"),
				use(message->socket,"socket");
		}
		else
		{
			*_sql <<SQLQueries::UpdateAdapter,
				use(message->adapterINTid,"ID"),
				use(std::to_string(message->fm_version),"FMver");
		}
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::UpdateAdapter");
		return (true);
	}
	catch(std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::UpdateAdapter");
		return (false);
	}
}

bool DBHandler::InsertSenAct(tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::InsertSenAct");
	try
	{
		std::string retRec;
		*_sql << SQLQueries::InsertFacility,
					use(message->sensor_id,"deviceID"),
					use(message->timestamp,"timestamp"),
					use(message->battery,"battery"),
					use(message->signal_strength,"signal"),
					use(message->adapterINTid,"adapterID");
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
			std::string val = "0";
			switch(message->values[i].type)
			{
				case TEMP:
				case LUM:
				case REZ:
				case POS:
				case BT:
					val = std::to_string(message->values[i].fval);
					break;
				case ONON:
				case TOG:
				case ONOFFSEN:
				case ONOFSW:
						if(message->values[i].bval)
							val = "1";
					break;
				case EMI:
				case HUM:
				case BAR:
				case RGB:
				case RAN:
				case BOT:
				case BOM:
				case BST:
					val = std::to_string(message->values[i].ival);
					break;
				default:
					this->_log->WriteMessage(WARN,"Unknown value type nothing will be saved to DB!");
					continue;
					break;
			}
			*_sql << SQLQueries::InsertDevice,
					use(message->sensor_id,"devID"),
					use(message->values[i].intType,"type"),
					use(val,"value");
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
		*_sql << SQLQueries::UpdateFacility,
			into(retRec),
			use(message->battery,"battery"),
			use(message->signal_strength, "quality"),
			use(message->sensor_id, "mac"),
			use(message->timestamp, "timestamp");
		for (int i = 0;i<message->values_count;i++)  //for all values create text representation and call SQL querry
		{

			std::string val = "0";
			switch(message->values[i].type)
			{
				case TEMP:
				case LUM:
				case REZ:
				case POS:
				case BT:
					val = std::to_string(message->values[i].fval);
					break;
				case ONON:
				case TOG:
				case ONOFFSEN:
				case ONOFSW:
						if(message->values[i].bval)
							val = "1";

					break;
				case EMI:
				case HUM:
				case BAR:
				case RGB:
				case RAN:
				case BOT:
				case BOM:
				case BST:
					val = std::to_string(message->values[i].ival);
					break;
				default:
					this->_log->WriteMessage(WARN,"Unknown value nothing will be saved to DB!");
					continue;
					break;
			}
			statement stl = (_sql->prepare <<SQLQueries::UpdateDevice,
								use(message->sensor_id,"deviceID"),
								use(val,"value"),
								use(message->values[i].intType,"type"));
			stl.execute(false);
			if (stl.get_affected_rows()<=0)
			{
				this->_log->WriteMessage(INFO,"Update affected 0 rows going to insert");
				*_sql << SQLQueries::InsertDevice,
						use(message->sensor_id,"devID"),
						use(message->values[i].intType,"type"),
						use(val,"value");
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

int DBHandler::GetWakeUpTime(std::string record)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetWakeUpTime");
	try
	{
		indicator ind;
		int retRec;
		*_sql << SQLQueries::SelectTime,
				into(retRec, ind),
				use(record) ;
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
	std::string sqlQuery = "SELECT value FROM devices Where (fk_facilities_mac='" + ID + "' AND type =" + type +");" ;
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
	std::string sqlQuery = "SELECT mail FROM users INNER JOIN users_adapters ON user_id=fk_user_id WHERE (fk_adapter_id=" + AdapterID + ");" ;
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

bool DBHandler::DeleteFacility(std::string ID)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::DeleteFacility");
	try
	{
		*_sql << SQLQueries::DeleteDevice,
				use(ID,"ID");
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


