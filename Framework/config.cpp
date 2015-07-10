/**
* @file config.cpp
*
* Implementace Metod pro zpracovani konfiguracniho souboru
*
* @author xrasov01
* @version 1.0
*/

#include "config.h"
#include "framework.h"

using namespace pugi;
using namespace std;

/** Konstruktor objektu tridy FrameworkConfig.
*
*/
FrameworkConfig::FrameworkConfig(){
}

/** Destruktor objektu tridy FrameworkConfig.
*
*/
FrameworkConfig::~FrameworkConfig(){
	this->ClearAlgorithms();
}

/**
* Metoda, ktera maze specifikace aplikacnich modulu.
*
*/
void FrameworkConfig::ClearAlgorithms(){
	for (auto oneAlgorithm = this->algorithms.begin(); oneAlgorithm != this->algorithms.end(); ++oneAlgorithm){
		delete (*oneAlgorithm);
	}
	this->algorithms.clear();
}

/**
* Nastavuje FrameworkConfig dle cesty zadane v parametru
*
* @param configPath   relativni cesta ke souboru s konfiguraci serveru
* @return             nothing
*/
void FrameworkConfig::SetConfig(std::string configPath){
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("config.xml");

	xml_node frameworkConfig = doc.child("framework_config");
	this->portUIServer = frameworkConfig.attribute("portUIServer").as_int();
	this->portAdaRecieverServer = frameworkConfig.attribute("portAdaRecieverServer").as_int();
	this->portAdaSenderServer = frameworkConfig.attribute("portAdaSenderServer").as_int();
	this->portAlgorithmServer = frameworkConfig.attribute("portAlgorithmsServer").as_int();
	this->recieveBuffSize = frameworkConfig.attribute("recieveBuffSize").as_int();
	this->dbName = frameworkConfig.attribute("DBName").value();
	this->maxNumberDBConnections = frameworkConfig.attribute("maxNumDBConnections").as_int();
	this->algorithmsConfig = frameworkConfig.attribute("algorithmsConfig").value();
	this->connectionPoolSize = frameworkConfig.attribute("connPoolSize").as_int();

	xml_node loger = frameworkConfig.child("loger");

	this->loggerSettingFileName = loger.attribute("fileName").value();
	this->loggerSettingAppName = loger.attribute("appName").value();
	this->loggerSettingVerbosity = loger.attribute("verbosity").as_int();
	this->loggerSettingFilesCnt = loger.attribute("filesCnt").as_int();
	this->loggerSettingLinesCnt = loger.attribute("LinesCnt").as_int();
	this->SetAlgorithms();
}
/**
* Nacte specifikaci algoritmu do objektu z konfiguracniho souboru
*
* @return            nothing
*/
void FrameworkConfig::SetAlgorithms(){
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("Algorithms/algsConfig.xml");

	xml_node algorithmsConfig = doc.child("algorithms_config");
	for (pugi::xml_node algNode = algorithmsConfig.first_child(); algNode; algNode = algNode.next_sibling())
	{
		talgorithm * tmpAlgorithm = new talgorithm();
		tmpAlgorithm->id = algNode.attribute("id").as_int();
		tmpAlgorithm->name = algNode.attribute("name").value();
		tmpAlgorithm->pathOfBinary = algNode.attribute("pathOfBinary").value();
		tmpAlgorithm->numParams = algNode.attribute("numParams").as_int();
		tmpAlgorithm->maxDevs = algNode.attribute("maxDevs").as_int();
		tmpAlgorithm->type = algNode.attribute("type").as_int();
		this->algorithms.push_back(tmpAlgorithm);


	}
}
/**
* Nastaveni instance pro logovani do objektu FrameworkConfig
*
* @param init_Log   instance tridy Loger, ktera umoznuje logovani do souboru
* @return            nothing
*/
void FrameworkConfig::SetLogger(Loger *init_Log){
	this->Log = init_Log;
}

/**
* Znovu nacte specifikaci algoritmu zadanych v souboru s konfiguraci.
*
* @return            nothing
*/
void FrameworkConfig::ResetAlgorithms(){
	Log->WriteMessage(TRACE, "ENTERING FrameworkConfig::ResetAlgorithms");

	this->ClearAlgorithms();
	this->SetAlgorithms();

	Log->WriteMessage(TRACE, "EXITING FrameworkConfig::ResetAlgorithms");
}

/**
* Vrati specifikaci jednoho algoritmu dle jeho id
*
* @param id           id algoritmu
* @return retValue    struktura talgorithm, ve ktere jsou ulozeny informace o algoritmu
*/
talgorithm * FrameworkConfig::GetAlgorithmById(int id){
	Log->WriteMessage(TRACE, "ENTERING FrameworkConfig::GetAlgorithmById");

	talgorithm * retValue = nullptr;
	for (auto oneAlgorithm = this->algorithms.begin(); oneAlgorithm != this->algorithms.end(); ++oneAlgorithm){
		if ((*oneAlgorithm)->id == id){
			Log->WriteMessage(TRACE, "EXITING FrameworkConfig::GetAlgorithmById");
			return (*oneAlgorithm);
		}
	}
	Log->WriteMessage(TRACE, "EXITING FrameworkConfig::GetAlgorithmById");
	return retValue;
}

/**
* Nastavi algoritmy do databaze
*
* @param database           objekt tridy DBFWHandler
*/
void FrameworkConfig::SetUpAlgorithmsInDatabase(DBFWHandler * database){
	Log->WriteMessage(TRACE, "ENTERING FrameworkConfig::SetUpAlgorithmsInDatabase");

	talgorithm * retValue = nullptr;
	for (auto oneAlgorithm = this->algorithms.begin(); oneAlgorithm != this->algorithms.end(); ++oneAlgorithm){
		database->UpdateAlgList(to_string((*oneAlgorithm)->id), (*oneAlgorithm)->name, to_string((*oneAlgorithm)->type));
		database->InsertAlgList(to_string((*oneAlgorithm)->id), (*oneAlgorithm)->name, to_string((*oneAlgorithm)->type));
	}
	Log->WriteMessage(TRACE, "EXITING FrameworkConfig::SetUpAlgorithmsInDatabase");
}

