/*
 * DBHandler.cpp
 *
 *  Created on: Oct 26, 2014
 *      Author: tuso
 */

#include "DBHandler.h"

	/**********************************************Start of DBHandler section******************************************************/

using namespace soci;

/** Metoda ktora zisti ci je zapnute logovanie pre dane zariadenie a ulozi logovacie data do databazy
 * @param message - spracovana sprava
    */

void DBHandler::LogValue (tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::LogValue");
	for (int i = 0; i<message->values_count;i++)
	{
		try  //zavolame prikaz na volzenie dat do databazy
		{
			std::string retRec;
			std::string val = "0";
			switch(message->values[i].type)
			{
				case TEMP:
				case LUM:
				case REZ:
				case POS:
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
					val = std::to_string(message->values[i].ival);
					break;
				default:
					this->_log->WriteMessage(WARN,"Unknown value type nothing will be saved to DB!");
					continue;
					break;
			}
			std::string sqlQuery = "insert into logs (fk_facilities_mac,timestamp,fk_devices_type,value) values ( '"+ message->DeviceIDstr + "', " + std::to_string(message->timestamp) + " , " + std::to_string(message->values[i].intType) + ", " + val + " );" ;
			this->_log->WriteMessage(TRACE,sqlQuery);
			*_sql << sqlQuery,
				into(retRec);
		}
		catch(std::exception const &e)
		{
			std::string ErrorMessage = "Database Error : ";
			ErrorMessage.append (e.what());
			this->_log->WriteMessage(ERR,ErrorMessage );
		}
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::LogValue");
}

/** Konstruktor objektu vytvoreneho z triedy DBHandler
 * @param DBname - nazov databazy
    */

DBHandler::DBHandler(soci::session *SQL, Loger *l)
{
	this->_log = l;
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	this->_sql = SQL;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}

/** Metoda pre zistenie pritomnosti urciteho zaznamu v databaze
 * @param tableName - nazov tabulky v ktorej sa ma zaznam hladat
 * @param columnName - stlpec podla ktoreho ma byt zaznam hladany
 * @param record - hodnota zaznamu
 * @return true/false podla toho ci bol zaznam najdeny
     */

bool DBHandler::IsInDB(std::string tableName, std::string columnName, std::string record)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::IsInDB");
	try //zavolame jednoduchy select nad DB
	{
		int retRec;
		std::string sqlQuery = "select count(*)" + columnName + " from " + tableName + " where " + columnName + " = "+ record + ";";
		this->_log->WriteMessage(TRACE,sqlQuery);
		*_sql << sqlQuery,
			into(retRec);
		if (retRec > 0) //ak sme dostali nejake data znamena to ze udaj sa uz v DB nachadza
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
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::IsInDB");
		return (false);
	}
}

/** Metoda vytvori v databaze novy zaznam v tabulke adapterov
 * @param message - obsah spracovanej spravy
 * @return na zaklade uspechu/neuspechu vrati true/false
    */

bool DBHandler::InsertAdapter(tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::InsertAdapter");
	try
	{
		std::string retRec;
        std::string addressStr = inet_ntoa(message->adapter_ip);
        std::string sqlQuery = "insert into adapters (adapter_id,version,ip_address) values ( "+ std::to_string(message->adapterINTid)+ ", " + std::to_string(message->fm_version) + ", '" + addressStr + "');" ;
		this->_log->WriteMessage(TRACE,sqlQuery);
		*_sql << sqlQuery,
			into(retRec);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertAdapter");
		return (true);
	}
	catch(std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertAdapter");
		return (false);
	}
}


/** Metoda aktualizuje zaznam v databaze v tabulke adapterov
 * @param message - obsah spracovanej spravy
 * @return na zaklade uspechu/neuspechu vrati true/false
    */

bool DBHandler::UpdateAdapter(tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::UpdateAdapter");
	try
	{
		std::string retRec;
        std::string addressStr = inet_ntoa(message->adapter_ip);
        std::string sqlQuery = "update adapters set version=" + std::to_string(message->fm_version) + ",ip_address='" + addressStr + "' where adapter_id=" + std::to_string(message->adapterINTid) + ";" ;
		this->_log->WriteMessage(TRACE,sqlQuery);
		*_sql << sqlQuery,
			into(retRec);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::UpdateAdapter");
		return (true);
	}
	catch(std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::UpdateAdapter");
		return (false);
	}
}


/** Metoda vytvori v databaze novy zaznam v tabulke zariadeni
 * @param message - obsah spracovanej spravy
 * @return na zaklade uspechu/neuspechu vrati true/false
    */

bool DBHandler::InsertSenAct(tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::InsertSenAct");
	try
	{
		std::string retRec;
		std::string sqlQuery = "insert into facilities (mac,refresh,battery,quality,fk_adapter_id,involved,timestamp) values \
				( '" + message->DeviceIDstr + "', 5 ," + std::to_string(message->battery)+ ", " + std::to_string(message->signal_strength) +  ", " +std::to_string(message->adapterINTid)+ ", " +std::to_string(message->timestamp) + ", " + std::to_string(message->timestamp) +" );" ;
		this->_log->WriteMessage(TRACE,sqlQuery);
		*_sql << sqlQuery,into(retRec);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error while inserting sensor/actor details : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertSenAct");
		return (false);
	}
	try
	{
		for (int i=0;i<message->values_count;i++)  //musime ulozit do DB vsetky hodnoty ktore sme dostali
		{
			std::string retRec;
			std::string val = "0";
			switch(message->values[i].type)
			{
				case TEMP:
				case LUM:
				case REZ:
				case POS:
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
					val = std::to_string(message->values[i].ival);
					break;
				default:
					this->_log->WriteMessage(WARN,"Unknown value type nothing will be saved to DB!");
					continue;
					break;
			}
			//pri prvom ulozeni do databazy sa nastavi cas zobudzania defaultne na 5 sekund
			std::string sqlQuery = "insert into devices (fk_facilities_mac,type,value) values ( '" + message->DeviceIDstr + "', " + std::to_string(message->values[i].intType) + ", '" + val + "');" ;
			this->_log->WriteMessage(TRACE,sqlQuery);
			*_sql << sqlQuery,
				into(retRec);
		}
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertSenAct");
		return (true);
	}
	catch(std::exception const &e)
	{
		std::string ErrorMessage = "Database Error while inserting values : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertSenAct");
		return (false);
	}
}


/** Metoda aktualizu v databaze zaznam v tabulke adapterov
 * @param message - obsah spracovanej spravy
 * @return na zaklade uspechu/neuspechu vrati true/false
    */

bool DBHandler::UpdateSenAct(tmessage *message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::UpdateSenAct");
	try
	{

		std::string retRec;
		std::string sqlQuery = "update facilities set battery=" + std::to_string(message->battery) + ",quality=" + std::to_string(message->signal_strength) + ",timestamp=" + std::to_string(message->timestamp)  +" where (mac='" + message->DeviceIDstr + "');" ;
		this->_log->WriteMessage(TRACE,sqlQuery);
		*_sql << sqlQuery,
			into(retRec);
		for (int i = 0;i<message->values_count;i++)
		{

			std::string val = "0";
			switch(message->values[i].type)
			{
				case TEMP:
				case LUM:
				case REZ:
				case POS:
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
					val = std::to_string(message->values[i].ival);
					break;
				default:
					this->_log->WriteMessage(WARN,"Unknown value nothing will be saved to DB!");
					continue;
					break;
			}
			std::string sqlQuery = "update devices set value=" + val  +" where (fk_facilities_mac='" + message->DeviceIDstr + "' AND type =" + std::to_string(message->values[i].intType) +");" ;
			this->_log->WriteMessage(TRACE,sqlQuery);
			*_sql << sqlQuery,
				into(retRec);
		}
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::UpdateSenAct");
		return (true);
	}
	catch(std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::UpdateSenAct");
		return (false);
	}
}


/** Metoda ziska z databazy zaznam o case zobudenia pre konkretny senzor
 * @param record - obsah spracovanej spravy
 * @return hodnota zobudenia
    */

int DBHandler::GetWakeUpTime(std::string record)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetWakeUpTime");
	try
	{
		//TODO indikator zabrani vyhozeni vyjimky, pokud nenajde ����dn�� data.
		indicator ind;
		int retRec;
		std::string sqlQuery = "select refresh from facilities where mac = '" + record + "';";
		*_sql << sqlQuery,
			into(retRec, ind);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetWakeUpTime");
		return (retRec);

	}
	catch(std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetWakeUpTime");
		return (5);
	}
}

void DBHandler::GetAdapterData(std::string *adapterIP, long int ID)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetAdapterData");
	std::string sqlQuery = "SELECT ip_address FROM adapters where adapter_id=" + std::to_string(ID) + ";" ;
	this->_log->WriteMessage(TRACE,sqlQuery);
	try
	{
		*_sql<<sqlQuery, into(*adapterIP);
	}
	catch(std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		this->_log->WriteMessage(ERR,ErrorMessage );
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


/** Destruktor objektu vytvoreneho z triedy DBHandler
 */

DBHandler::~DBHandler()
{
	//delete (_sql); //zrusenie a vymazanie pripojenia k DB
}

	/**********************************************End of DBHandler section******************************************************/


