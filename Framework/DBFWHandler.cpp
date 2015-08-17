/**
* @file DBFWHandler.cpp
*
* Implementace metod, krete pracuji jiz primo nad DB. Jde o abstrakci dotazu nad Databazi nad tabulkami pro algoritmy a dalsimi na nich navazanymi. Vyuziva se knihovny soci (soci::session)
*
* @author xrasov01
* @version 1.0
*/

#include "DBFWHandler.h"
using namespace soci;

/**
* Vrati sezeni nad databazi pro pripadne uvolneni z pameti nebo navraceni do kolekce sezeni.
*
* @return session	ukazatel na sezeni
*/
soci::session *DBFWHandler::GetConnectionSession()
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::ReturnConnection");
	session * t = this->ses;
	this->ses = NULL;
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::ReturnConnection");
	return t;
}

/** Konstruktor tridy DBFWHandler
*
* @param session	Sezeni nad databazi
* @param loger		Ukazatel na instanci logeru umoznujici logovani do souboru.
*/
DBFWHandler::DBFWHandler(soci::session *ses_init, Loger *loger)
{
	//this->_log->WriteMessage(TRACE, "Entering DBFWHandler::Constructor");
	this->_log = loger;
	this->ses = ses_init;
	//this->_log->WriteMessage(TRACE, "Exiting DBFWHandler::Constructor");
}

/** Destruktor tridy DBFWHandler
*
*/
DBFWHandler::~DBFWHandler()
{
}

//---------------------------------------  BEGIN QUERY SECTION ---------------------------------------

/** Dotaz ziskavajici posledni hodnotu z tabulky Devices (napr pro ziskani posledni teploty z teplotniho senzoru)
*
* @param ID			fk_facilities_mac
* @param type		type
*/
float DBFWHandler::GetValueFromModule(std::string ID)
{
	double retVal;
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::GetLastTemp");
	std::string sqlQuery = "SELECT measured_value FROM module Where (device_mac=:mac);";
	this->_log->WriteMessage(TRACE, sqlQuery);
	try
	{
		*ses << sqlQuery, use(ID), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		retVal = 0.0;
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::GetLastTemp");
	return (retVal);
}

/** Dotaz nastavujici posledni hodnotu (domena value) v tabulce Module. (dle parametru)
*
* @param ID			fk_facilities_mac
* @param type		type
* @param value		Hodnota, na kterou se provadi Update
*
* @return boolean dle toho zda update probehl uspesne
*/
bool DBFWHandler::UpdateValueOfModule(std::string ID, std::string value)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::UpdateLastTemp");
	std::string retVal = "";
	try
	{
		std::string sqlQuery = "UPDATE module SET  measured_value=:value  Where (device_mac=:mac);";
		this->_log->WriteMessage(TRACE, sqlQuery);
		*ses << sqlQuery, use(value), use(ID),
			into(retVal);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::UpdateLastTemp");
		return (true);

	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::UpdateLastTemp");
		return (false);
	}
}

/** Dotaz ziskavajici Email z tabulky Users dle primarniho klice (id)
*
* @param UserID			user_id
*
* @return email
*/
std::string DBFWHandler::GetEmailByUserId(std::string UserID)
{
	std::string retVal = "";
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::GetEmailByUserId");
	std::string sqlQuery = "SELECT mail FROM users WHERE user_id=:userId;";
	this->_log->WriteMessage(TRACE, sqlQuery);
	try
	{
		*ses << sqlQuery,use(UserID), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		retVal = "error";
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::GetEmailByUserId");
	return (retVal);
}

/** Dotaz ziskavajici primarni klic z tabulky Users dle Emailu
*
* @param Email			mail
*
* @return user_id
*/
std::string DBFWHandler::GetUserIdByEmail(std::string Email)
{
	std::string retVal = "";
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::GetEmailByUserId");
	std::string sqlQuery = "SELECT user_id FROM users WHERE mail = :mail;";
	this->_log->WriteMessage(TRACE, sqlQuery);
	try
	{
		*ses << sqlQuery, use(Email), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		retVal = "0";
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::GetEmailByUserId");

	return (retVal);
}

/** Dotaz ziskavajici GCM Identifikacni retezce pro jednoho uzivatele a vracejici jako vektor retezcu.
*
* @param userId			user_id
*
* @return vektor retezcu GCM identifikacnich retezcu
*/
std::vector<std::string> DBFWHandler::GetNotifStringByUserId(std::string userId)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::GetNotifString");
	std::string sqlQuery = "SELECT push_notification FROM users INNER JOIN mobile_devices ON user_id=fk_user_id WHERE (user_id=:userId);";
	this->_log->WriteMessage(TRACE, sqlQuery);
	std::vector<std::string> retVal(20);
	try
	{
		*ses << sqlQuery, use(userId), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::GetNotifString");
	return (retVal);
}

/** Dotaz mazajici zaznam v tabulce mobile_devices dle GCM ID
*
* @param UsersAlgorithmsId			fk_users_algorithms_id
*
* @return  pravdivostni hodnota zda se smazani zaznamu podarilo
*/
bool DBFWHandler::DeleteMobileDeviceByGcmId(std::string GCMId)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::DeleteMobileDeviceByGcmId");
	std::string retVal = "";
	try
	{
		std::string sqlQuery = "DELETE FROM mobile_devices WHERE push_notification = :gcmid;";
		this->_log->WriteMessage(TRACE, sqlQuery);
		*ses << sqlQuery, use(GCMId),
			into(retVal);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::DeleteMobileDeviceByGcmId");
		return (true);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::DeleteAlgoDevices");
		return (false);
	}
}

/** Dotaz ziskavajici vsechny primarni klice algoritmu, ktere jsou navazany na adapter a vracejici jako vektor retezcu (musi se nasledne pretypovat!). Informace se vyhledavaji z tabulky users_algorithms
*
* @param adapterId			fk_adapter_id
*
* @return vsechny primarni klice algoritmu
*/
std::vector<std::string> DBFWHandler::SelectAllIdsOfUsersAlgorithmsByAdapterId(std::string adapterId)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectAllIdsOfUsersAlgorithmsByAdapterId");
	std::string sqlQuery = "SELECT users_algorithms_id FROM users_algorithms WHERE (fk_adapter_id=:adapterId);";
	this->_log->WriteMessage(TRACE, sqlQuery);
	std::vector<std::string> retVal(20);
	try
	{
		*ses << sqlQuery, use(adapterId), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectAllIdsOfUsersAlgorithmsByAdapterId");
	return (retVal);
}

/** Dotaz ziskavajici vsechny primarni klice algoritmu, ktere jsou navazany na adapter a na uzivatele a vracejici jako vektor retezcu (musi se nasledne pretypovat!). Informace se vyhledavaji z tabulky users_algorithms
*
* @param adapterId			fk_adapter_id
* @param userId				fk_user_id
*
* @return vsechny primarni klice algoritmu
*/
std::vector<std::string> DBFWHandler::SelectAllIdsOfUsersAlgorithmsByAdapterIdAndUserId(std::string adapterId, std::string userId)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectAllIdsOfUsersAlgorithmsByAdapterIdAndUserId");
	std::string sqlQuery = "SELECT users_algorithms_id FROM users_algorithms WHERE (fk_adapter_id=:adapterId AND fk_user_id =:userId);";
	this->_log->WriteMessage(TRACE, sqlQuery);
	std::vector<std::string> retVal(20);
	try
	{
		*ses << sqlQuery, use(adapterId), use(userId), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectAllIdsOfUsersAlgorithmsByAdapterIdAndUserId");
	return (retVal);
}

/** Dotaz ziskavajici vsechny primarni klice algoritmu, ktere jsou povolene (enabled == true) a dle adapter id
*
* @param adapterId			fk_adapter_id
*
* @return primarni klice algoritmu
*/
std::vector<std::string> DBFWHandler::SelectIdsEnabledAlgorithmsByAdapterId(std::string adapterId)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectIdsEnabledAlgorithmsByAdapterId");
	std::string sqlQuery = "SELECT users_algorithms_id FROM users_algorithms WHERE (fk_adapter_id=:adapterId AND state = '1');";
	this->_log->WriteMessage(TRACE, sqlQuery);
	std::vector<std::string> retVal(20);
	try
	{
		*ses << sqlQuery, use(adapterId), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectIdsEnabledAlgorithmsByAdapterId");
	return (retVal);
}

/** Dotaz ziskavajici vsechny primarni klice algoritmu, ktere jsou urciteho typu (algId nebo AlgType)
*
* @param algId			fk_algorithm_id
*
* @return primarni klice algoritmu
*/
std::vector<std::string> DBFWHandler::SelectIdsAlgorithmsByAlgIdAndUserId(std::string userId, std::string algId)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectIdsAlgorithmsByAlgId");
	std::string sqlQuery = "SELECT users_algorithms_id FROM users_algorithms WHERE (fk_user_id = :algoId AND fk_algorithm_id=:userId); ";
	this->_log->WriteMessage(TRACE, sqlQuery);
	std::vector<std::string> retVal(20);
	try
	{
		*ses << sqlQuery, use(algId), use(userId), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectIdsAlgorithmsByAlgId");
	return (retVal);
}

/** Dotaz ziskavajici typ algoritmu konkterniho uzivatelskeho algoritmu (z tabulky users_algorithms)
*
* @param UsersAlgId			users_algorithms_id
*
* @return id typu algoritmu
*/
std::string DBFWHandler::SelectAlgIdByUsersAlgId(std::string UsersAlgId)
{
	std::string retVal = "";
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectAlgIdByUsersAlgId");
	std::string sqlQuery = "SELECT fk_algorithm_id FROM users_algorithms WHERE users_algorithms_id = :usersAlgoId;";
	this->_log->WriteMessage(TRACE, sqlQuery);
	try
	{
		*ses << sqlQuery, use(UsersAlgId), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		retVal = "0";
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectAlgIdByUsersAlgId");
	return (retVal);
}

/** Dotaz ziskavajici stav (state) konkterniho uzivatelskeho algoritmu (z tabulky users_algorithms)
*
* @param UsersAlgId			users_algorithms_id
*
* @return stav (state) algoritmu
*/
std::string DBFWHandler::SelectStateByUsersAlgId(std::string UsersAlgId)
{
	std::string retVal = "";
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectStateByUsersAlgId");
	std::string sqlQuery = "SELECT state FROM users_algorithms WHERE users_algorithms_id = :usersAlgoId;";
	this->_log->WriteMessage(TRACE, sqlQuery);
	try
	{
		*ses << sqlQuery, use(UsersAlgId), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		retVal = "0";
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectStateByUsersAlgId");
	return (retVal);
}

/** Dotaz ziskavajici jedinecne ID Adapteru na ktery je navazan uzivatelsky algoritmus
*
* @param UsersAlgId			users_algorithms_id
*
* @return jedinecne ID Adapteru
*/
std::string DBFWHandler::SelectAdapterIdByUsersAlgId(std::string UsersAlgId)
{
	std::string retVal = "";
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectStateByUsersAlgId");
	std::string sqlQuery = "SELECT fk_adapter_id FROM users_algorithms WHERE users_algorithms_id = :usersAlgoId;";
	this->_log->WriteMessage(TRACE, sqlQuery);
	try
	{
		*ses << sqlQuery, use(UsersAlgId), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		retVal = "0";
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectStateByUsersAlgId");
	return (retVal);
}

/** Dotaz ziskavajici ID Uzivatele dle Uzivatelskeho algoritmu
*
* @param UsersAlgId			users_algorithms_id
*
* @return ID Uzivatele
*/
std::string DBFWHandler::SelectUserIdByUsersAlgId(std::string UsersAlgId)
{
	std::string retVal = "";
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectAlgIdByUsersAlgId");
	std::string sqlQuery = "SELECT fk_user_id FROM users_algorithms WHERE users_algorithms_id = :usersAlgoId;";
	this->_log->WriteMessage(TRACE, sqlQuery);
	try
	{
		*ses << sqlQuery, use(UsersAlgId), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		retVal = "0";
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectAlgIdByUsersAlgId");
	return (retVal);
}

/** Dotaz ziskavajici jmeno uzivatelem nadefinovane uzivatelskeho algoritmu dle ID Uzivatelskeho algoritmu
*
* @param UsersAlgId			users_algorithms_id
*
* @return  jmeno algoritmu (uzivatelem nadefinovane)
*/
std::string DBFWHandler::SelectNameByUsersAlgId(std::string UsersAlgId)
{
	std::string retVal = "";
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectNameByUsersAlgId");
	std::string sqlQuery = "SELECT name FROM users_algorithms WHERE users_algorithms_id = :usersAlgoId;";
	this->_log->WriteMessage(TRACE, sqlQuery);
	try
	{
		*ses << sqlQuery, use(UsersAlgId), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		retVal = "0";
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectNameByUsersAlgId");
	return (retVal);
}

/** Dotaz ziskavajici parametry uzivatelskeho algoritmu (users_algorithms) dle users_algorithms ID
*
* @param UsersAlgId			users_algorithms_id
*
* @return  parametry
*/
std::string DBFWHandler::SelectParametersByUsersAlgId(std::string UsersAlgId)
{
	std::string retVal = "";
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectParametersByUsersAlgId");
	std::string sqlQuery = "SELECT parameters FROM users_algorithms WHERE users_algorithms_id = :usersAlgoId;";
	this->_log->WriteMessage(TRACE, sqlQuery);
	try
	{
		*ses << sqlQuery, use(UsersAlgId),into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		retVal = "";
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectParametersByUsersAlgId");
	return (retVal);
}

/** Dotaz ziskavajici seznam ID senzoru/aktoru navazane na uzivatelsky algoritmus.
*
* @param UsersAlgId			users_algorithms_id
*
* @return  seznam ID senzoru/aktoru jako vektor stringu (musi se nasledne ID pretypovat!)
*/
std::vector<std::string> DBFWHandler::SelectDevIdsByUsersAlgId(std::string UsersAlgId)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectDevIdsByUsersAlgId");
	std::string sqlQuery = "SELECT fk_facilities_mac FROM algo_devices WHERE (fk_users_algorithms_id=:usersAlgoId);";
	this->_log->WriteMessage(TRACE, sqlQuery);
	std::vector<std::string> retVal(20);
	try
	{
		*ses << sqlQuery, use(UsersAlgId),into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectDevIdsByUsersAlgId");
	return (retVal);
}

/** Dotaz ziskavajici Typ (type) zarizeni (senzoru/aktoru) dle ID zarizeni
*
* @param devId			fk_facilities_mac
*
* @return  typ zarizeni (jako string)
*/
std::string DBFWHandler::SelectDevTypeByDevId(std::string devId)
{
	std::string retVal = "";
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::SelectDevTypeByDevId");
	std::string sqlQuery = "SELECT fk_devices_type FROM algo_devices WHERE fk_facilities_mac = :mac;";
	this->_log->WriteMessage(TRACE, sqlQuery);
	try
	{
		*ses << sqlQuery, use(devId), into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		retVal = "";
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::SelectDevTypeByDevId");
	return (retVal);
}

/** Dotaz vkladajici Uzivatelsky algoritmus (users_algorithms) do databaze.
*
* @param userId				fk_user_id
* @param algId				fk_algorithm_id
* @param parameters			parameters
* @param name				name
* @param adapterId			fk_adapter_id
*
* @return  ID noveho zaznamu users_algorithms v Databazi
*/

std::string DBFWHandler::InsertUserAlgorithm(std::string userId, std::string algId, std::string parameters, std::string name, std::string adapterId)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::InsertUserAlgorithm");
	std::string retVal = "";
	try
	{
		std::string sqlQuery = "INSERT into users_algorithms (fk_user_id,fk_algorithm_id, parameters,data,name,fk_adapter_id,state) values ( :userId, :algoId, :parameters, '',:anme, :adapterId, '1') returning users_algorithms_id;";
		this->_log->WriteMessage(TRACE, sqlQuery);
		*ses << sqlQuery,
		 use(userId), use(algId), use(parameters), use(name), use(adapterId),
			into(retVal);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::InsertUserAlgorithm");

	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::InsertUserAlgorithm");
		retVal = "0";
	}
	return (retVal);
}

/** Dotaz menici hodnoty uzivatelskeho algoritmu (users_algorithms)
*
* @param userAlgId			users_algorithms_id
* @param algId				fk_algorithm_id
* @param parameters			parameters
* @param name				name
* @param adapterId			fk_adapter_id
*
* @return  pravdivostni hodnota zda se editace zaznamu podarila
*/
bool DBFWHandler::UpdateUserAlgorithm(std::string userAlgId, std::string AlgId, std::string parameters, std::string name, std::string state)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::UpdateUserAlgorithm");
	std::string retVal = "";
	try
	{
		std::string sqlQuery = "UPDATE users_algorithms SET  fk_algorithm_id=:algoId, parameters = :parameters, name = :name, state = :state WHERE (users_algorithms_id = :usersAlgoId);";
		this->_log->WriteMessage(TRACE, sqlQuery);
		*ses << sqlQuery,
		 use(AlgId), use(parameters), use(name), use(state), use(userAlgId),
			into(retVal);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::UpdateUserAlgorithm");
		return (true);

	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::UpdateUserAlgorithm");
		return (false);
	}
}

/** Dotaz vkladajici zaznam algo_devices do databaze
*
* @param UsersAlgorithmsId			fk_users_algorithms_id
* @param UserId						fk_user_id
* @param AlgorithId					fk_algorithm_id
* @param mac						fk_facilities_mac
* @param type						fk_devices_type
*
* @return  pravdivostni hodnota zda se vlozeni zaznamu podarilo
*/
bool DBFWHandler::InsertAlgoDevices(std::string UsersAlgorithmsId, std::string UserId, std::string AlgorithId, std::string mac, std::string type)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::InsertAlgoDevices");
	std::string retVal = "";
	try
	{
		std::string sqlQuery = "INSERT into algo_devices (fk_users_algorithms_id,fk_user_id,fk_algorithm_id,fk_facilities_mac,fk_devices_type) values ( :usersAlgoId, :userId, :algoId, :mac, :type );";
		this->_log->WriteMessage(TRACE, sqlQuery);
		*ses << sqlQuery,
		 use(UsersAlgorithmsId), use(UserId), use(AlgorithId), use(mac), use(type), 
			into(retVal);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::InsertAlgoDevices");
		return (true);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::InsertAlgoDevices");
		return (false);
	}
}

/** Dotaz mazajici vsechny zaznamy z algo_devices z databaze dle ID uzivatelskeho algoritmu
*
* @param UsersAlgorithmsId			fk_users_algorithms_id
*
* @return  pravdivostni hodnota zda se smazani zaznamu podarilo
*/
bool DBFWHandler::DeleteAlgoDevices(std::string UsersAlgorithmsId)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::DeleteAlgoDevices");
	std::string retVal = "";
	try
	{
		std::string sqlQuery = "DELETE FROM algo_devices WHERE fk_users_algorithms_id = :usersAlgoId;";
		this->_log->WriteMessage(TRACE, sqlQuery);
		*ses << sqlQuery,
		use(UsersAlgorithmsId),
			into(retVal);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::DeleteAlgoDevices");
		return (true);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::DeleteAlgoDevices");
		return (false);
	}
}

/** Dotaz mazajici zaznam users_algorithms z databaze dle primarniho klice
*
* @param UsersAlgorithmsId			users_algorithms_id
*
* @return  pravdivostni hodnota zda se smazani zaznamu podarilo
*/
bool DBFWHandler::DeleteUsersAlgorithms(std::string UsersAlgorithmsId)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::DeleteUsersAlgorithms");
	std::string retVal = "";
	try
	{
		std::string sqlQuery = "DELETE FROM users_algorithms WHERE users_algorithms_id = :usersAlgoId;";
		this->_log->WriteMessage(TRACE, sqlQuery);
		*ses << sqlQuery, use(UsersAlgorithmsId),
			into(retVal);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::DeleteUsersAlgorithms");
		return (true);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::DeleteUsersAlgorithms");
		return (false);
	}
}

/** Dotaz vkladajici zaznam do tabulky notifications (reprezentuje LOGOVANI NOTIFIKACI)
*
* @param text				text notifikace
* @param level				level
* @param timestamp			timestamp - Unix timestamp (ms), kdy byla zprava vytvorena
* @param userId				fk_user_id
* @param messageId			message_id - unikatni identiffikator zpravy
* @param read				read - urcuje zda byla zprava jiz prectena
* @param name				name
*
* @return  pravdivostni hodnota zda se vlozeni zaznamu do DB podarilo
*/
bool DBFWHandler::InsertNotification(std::string text, std::string level, std::string timestamp, std::string userId,
									std::string messageId, std::string read, std::string name)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::InsertNotification");
	std::string retVal = "";
	try
	{
		std::string sqlQuery = "INSERT into notifications (text,level,message_id,timestamp,fk_user_id,read,name) values ( :text, :level, :msgId, :timestamp, :userId, :read, :name );";
		this->_log->WriteMessage(TRACE, sqlQuery);
		*ses << sqlQuery,
		 use(text), use(level), use(messageId), use(timestamp), use(userId), use(read), use(name),
			into(retVal);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::InsertNotification");
		return (true);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::InsertNotification");
		return (false);
	}
}

/** Dotaz ziskajici maximalni hodnotu ID Notifikace dle ID Uzivatele
*
* @param userId				fk_user_id
*
* @return  nejvyssi hodnota ID notifikace dle uzivatele
*/
std::string DBFWHandler::GetHighestIdInNotification()
{
	std::string retVal = "";
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::GetHighestIdInNotificationsPerUser");
	std::string sqlQuery = "SELECT message_id FROM notifications ORDER BY message_id DESC LIMIT 1;";
	this->_log->WriteMessage(TRACE, sqlQuery);
	try
	{
		*ses << sqlQuery, into(retVal);
	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		retVal = "0";
	}
	this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::GetHighestIdInNotificationsPerUser");
	return (retVal);
}

/** Dotaz vkladajici nazev a ID aplikacniho modulu do tabulky u_algorithms
*
* @param algorithm_id		algorithm_id
* @param name				nazev
* @param type				typ
*
* @return  pravdivostni hodnota provedeni
*/
bool DBFWHandler::InsertAlgList(std::string algorithm_id, std::string name, std::string type)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::InsertAlgList");
	std::string retVal = "";
	try
	{
		std::string sqlQuery = "INSERT into u_algorithms (algorithm_id,name, type) values ( :algoId, :name, :type); ";
		this->_log->WriteMessage(TRACE, sqlQuery);
		*ses << sqlQuery, use(algorithm_id), use(name), use(type),
			into(retVal);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::InsertAlgList");
		return (true);

	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::InsertAlgList");
		return (false);
	}
}

/** Dotaz UPDATE dle ID aplikacniho modulu v tabulce u_algorithms
*
* @param ID			fk_facilities_mac
* @param type		type
* @param value		Hodnota, na kterou se provadi Update
*
* @return boolean dle toho zda update probehl uspesne
*/
bool DBFWHandler::UpdateAlgList(std::string algorithm_id, std::string name, std::string type)
{
	this->_log->WriteMessage(TRACE, "Entering " + this->_Name + "::UpdateAlgList");
	std::string retVal = "";
	try
	{
		std::string sqlQuery = "UPDATE u_algorithms SET name=:name, type = :type WHERE (algorithm_id=:algoId);";
		this->_log->WriteMessage(TRACE, sqlQuery);
		*ses << sqlQuery, use(name), use(type), use(algorithm_id),
			into(retVal);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::UpdateAlgList");
		return (true);

	}
	catch (std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append(e.what());
		this->_log->WriteMessage(ERR, ErrorMessage);
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::UpdateAlgList");
		return (false);
	}
}

//---------------------------------------  END QUERY SECTION ---------------------------------------


