/**
* @file config.cpp
*
* @Implementace Metod pro zpracovani konfiguracniho souboru
*
* @author xrasov01
* @version 1.0
*/

#include "config.h"
#include "framework.h"

using namespace pugi;
using namespace std;

FrameworkConfig::FrameworkConfig(){
}

FrameworkConfig::~FrameworkConfig(){
	this->ClearAlgorithms();
}

void FrameworkConfig::ClearAlgorithms(){
	for (auto oneAlgorithm = this->algorithms.begin(); oneAlgorithm != this->algorithms.end(); ++oneAlgorithm){
		delete (*oneAlgorithm);
	}
	this->algorithms.clear();
}

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

	xml_node loger = frameworkConfig.child("loger");
	
	this->loggerSettingFileName = loger.attribute("fileName").value();
	this->loggerSettingAppName = loger.attribute("appName").value();
	this->loggerSettingVerbosity = loger.attribute("verbosity").as_int();
	this->loggerSettingFilesCnt = loger.attribute("filesCnt").as_int();
	this->loggerSettingLinesCnt = loger.attribute("LinesCnt").as_int();
	this->SetAlgorithms();
}

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

void FrameworkConfig::SetLogger(Loger *init_Log){
	this->Log = init_Log;
}

void FrameworkConfig::ResetAlgorithms(){
	Log->WriteMessage(TRACE, "ENTERING FrameworkConfig::ResetAlgorithms");

	this->ClearAlgorithms();
	this->SetAlgorithms();

	Log->WriteMessage(TRACE, "EXITING FrameworkConfig::ResetAlgorithms");
}

talgorithm * FrameworkConfig::GetAlgorithmById(int id){
	Log->WriteMessage(TRACE, "ENTERING FrameworkConfig::GetAlgorithmById");

	talgorithm * retValue = NULL;
	for (auto oneAlgorithm = this->algorithms.begin(); oneAlgorithm != this->algorithms.end(); ++oneAlgorithm){
		if ((*oneAlgorithm)->id == id){
			Log->WriteMessage(TRACE, "EXITING FrameworkConfig::GetAlgorithmById");
			return (*oneAlgorithm);
		}
	}
	Log->WriteMessage(TRACE, "EXITING FrameworkConfig::GetAlgorithmById");
	return retValue;
}

