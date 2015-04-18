/**
* @file config.h
*
* @Definice tridy FrameworkConfig
*
* @author xrasov01
* @version 1.0
*/
#include <string>
#include <stdlib.h> 
#include "../loger.h"

#ifndef STRUCTURES_CONFIG_
#define STRUCTURES_CONFIG_

typedef struct algorithm
{
	int id;
	std::string name;
	std::string pathOfBinary;
	int numParams;
	int maxDevs;
	int type;
} talgorithm;

class FrameworkConfig
{
private:
	Loger *Log;
public:
	FrameworkConfig();
	~FrameworkConfig();
	void SetConfig(std::string configPath);
	void SetAlgorithms();
	void ClearAlgorithms();
	void SetLogger(Loger *init_Log);
	void ResetAlgorithms();
	talgorithm * GetAlgorithmById(int id);
	int portUIServer;
	int portAdaRecieverServer;
	int portAdaSenderServer;
	int portAlgorithmServer;
	int recieveBuffSize;
	int maxNumberDBConnections;
	std::string dbName;
	std::string algorithmsConfig;
	std::string loggerSettingFileName;
	std::string loggerSettingAppName;
	int loggerSettingVerbosity;			//Mira vypisovani hlaseni
	int loggerSettingFilesCnt;
	int loggerSettingLinesCnt;
	std::vector<talgorithm *> algorithms;
};

#endif