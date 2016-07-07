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

void DBHandler::LogValue (tmessageV1_0 *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::LogValue");
	for (int i = 0; i<message->values_count;i++) //for all values create text representation and call SQL querry
	{
		try
		{
			std::string retRec;
			//std::string val = "0";
			std::string val = std::to_string(message->values[i].measured_value);
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
		std::string sqlQuery = "select count(*) from " + tableName + " where " + columnName + " = "+ record + ";";
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

bool DBHandler::InsertGateway(tmessageV1_0 *message)
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

bool DBHandler::UpdateGateway(tmessageV1_0 *message)
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

bool DBHandler::InsertSenAct(tmessageV1_0 *message)
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
	return(true);
  //following lines is replaced by DB trigger
  /*
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
	} */
}

bool DBHandler::UpdateSenAct(tmessageV1_0 *message)
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
    
      std::string moduleStatus = message->values[i].status;
                                 
  				this->_log->WriteMessage(INFO,moduleStatus);
      if(moduleStatus == "available")
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
      else if(moduleStatus == "unavailable")
      {
  			statement stl = (_sql->prepare<<SQLQueries::UpdateModuleWithStatus,
  								use(message->device_euid,"DEVICE_EUID"),
  								use(moduleStatus, "STATUS"),
  								use(message->values[i].module_id,"MODULE_ID"),
                  use(message->adapterINTid, "GATEWAY_ID"));
  			stl.execute(false);
      }
      else
      {
           this->_log->WriteMessage(ERR, "unknown device status: " + moduleStatus);
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

unsigned int DBHandler::GetWakeUpTime(std::string record, long long int gateway_id)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetWakeUpTime");
	try
	{
		indicator ind;
		unsigned int retRec;
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

std::string DBHandler::GetXmlDeviceParameters(std::string device_euid, long long int adapterid)
{        
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetXmlDeviceParameters");
  
  std::string xmlParameters;
                                                                                           
	try                                                                                      
	{                                                                                        
			indicator ind;                                                                       
			*_sql << SQLQueries::SelectDeviceParametersXml,                                      
					into(xmlParameters, ind),                                                        
					use(adapterid), use(device_euid);                                                
                                                                                           
	}                                                                                        
	catch(std::exception const &e)                                                           
	{                                                                                        
			this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );        
			std::string ErrorMessage = "Database Error : ";                                      
			ErrorMessage.append (e.what());                                                      
			this->_log->WriteMessage(ERR,ErrorMessage );                                         
			xmlParameters == "";                                                                 
	}                                                                                        
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetXmlDeviceParameters");   
  return xmlParameters; 
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

bool DBHandler::GetDevices(tmessageV1_1 *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetDevices");
	size_t count = 0;
	try
	{
		*_sql << SQLQueries::SelectAllDevicesCount,
				use(message->adapterINTid, "GATEWAY_ID"),
				use(DEVICE_INITIALIZED, "DEVICE_INITIALIZED"),
				into(count);
		message->params->at(message->processedParams)->deviceList = new std::vector<unsigned long long>(count);
		message->params->at(message->processedParams)->deviceIDList = new std::vector<unsigned long long>(count);
		*_sql << SQLQueries::SelectAllDevices,
				use(message->adapterINTid, "GATEWAY_ID"),
				use(DEVICE_INITIALIZED, "DEVICE_INITIALIZED"),
				into(*(message->params->at(message->processedParams)->deviceList)),
				into(*(message->params->at(message->processedParams)->deviceIDList));
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetDevices");
		return (true);
	}
	catch(std::exception const &e)
	{
		this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetDevices");
		return (false);
	}
}

bool DBHandler::GetLastModuleValue(tmessageV1_1 *message)
{
        this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetLastModuleValue");
        size_t count = 1;
        try
        {
                std::string sqlTrace = "select measured_value from module where gateway_id = " + std::to_string(message->adapterINTid);
                sqlTrace += " and device_euid = " + std::to_string(message->device_euid) + " and module_id = " + std::to_string(message->module_id);
                this->_log->WriteMessage(TRACE, sqlTrace);
                message->params->at(message->processedParams)->deviceList = new std::vector<unsigned long long>(count);
                *_sql << SQLQueries::SelectLastModuleValue,
                                use(message->adapterINTid, "GATEWAY_ID"),
                                use(message->params->at(message->processedParams)->module_id, "MODULE_ID"),
                                use(message->params->at(message->processedParams)->euid, "DEVICE_EUID"),
                                into((message->params->at(message->processedParams)->measured_value)),
                this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetLastModuleValue");
                return (true);
        }
        catch(std::exception const &e)
        {
                this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
                std::string ErrorMessage = "Database Error : ";
                ErrorMessage.append (e.what());
                this->_log->WriteMessage(ERR,ErrorMessage );
                this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetLastValue");
                return (false);
        }
}

bool DBHandler::GetUserLabelForDevice(tmessageV1_1 *message)
{
        this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetUserLaberForDevice");
        try
        {
                std::string sqlTrace = "select device_name from device where gateway_id = " + std::to_string(message->adapterINTid);
                sqlTrace += " and device_euid = " + std::to_string(message->params->at(message->processedParams)->euid);
                this->_log->WriteMessage(TRACE, sqlTrace);
                *_sql << SQLQueries::SelectUserLabelForDeviceID,
                                use(message->adapterINTid, "GATEWAY_ID"),
                                use(message->params->at(message->processedParams)->euid, "DEVICE_EUID"),
                                into((message->params->at(message->processedParams)->value));
                this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetUserLaberForDevice");
                return (true);
        }
        catch(std::exception const &e)
        {
                this->_log->WriteMessage(ERR, "Error in query : " + _sql->get_last_query() );
                std::string ErrorMessage = "Database Error : ";
                ErrorMessage.append (e.what());
                this->_log->WriteMessage(ERR,ErrorMessage );
                this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetUserLaberForDevice");
                return (false);
        }
}
