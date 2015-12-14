#include "Config.h"

static const char configRoot[] = "config";
static const char serverNode[] = "server";
static const char databaseNode[] = "database";
static const char comTableNode[] = "comTable";
static const char logsNode[] = "logs";
static const char sslNode[] = "ssl";

using namespace std;

Config::Config() 
{
}

Config::~Config() 
{
}

void Config::loadXml(std::string file) 
{

    pugi::xml_parse_result result = _doc.load_file(file.c_str());
    if(!result){
        std::string desc = result.description();
        throw "Cant load config XML file:"+file+" Error description: " + desc + "\n"; 
    }
    _threadNumber = _doc.child(configRoot).child(serverNode).attribute("threads").as_int(1);
    _serverPort = _doc.child(configRoot).child(serverNode).attribute("port").as_int();
    
    _adaServerPort = _doc.child(configRoot).child(serverNode).attribute("adaServerPort").as_int();
    _algorithmPort = _doc.child(configRoot).child(serverNode).attribute("algorithmPort").as_int();
    _gamificationPort = _doc.child(configRoot).child(serverNode).attribute("gamificationPort").as_int();
    _verbosity = _doc.child(configRoot).child(serverNode).attribute("verbosity").as_int(10);
    string temp = _doc.child(configRoot).child(serverNode).attribute("debugMode").as_string();
    _debugMode = _doc.child(configRoot).child(serverNode).attribute("debugMode").as_bool(false);
    _connectionString = _doc.child(configRoot).child(databaseNode).attribute("connectionString").as_string("");
    _sessionsNumber = _doc.child(configRoot).child(databaseNode).attribute("sessions").as_int(10);
    _logsToCout = _doc.child(configRoot).child(logsNode).attribute("toCout").as_bool(true);
    _logsFolder = _doc.child(configRoot).child(logsNode).attribute("folder").as_string("logs");
    
    _comTableSleepPeriodMs = _doc.child(configRoot).child(comTableNode).attribute("cleanerSleepPeriodMs").as_int(1000);
    _comTableMaxInactivityMs =  _doc.child(configRoot).child(comTableNode).attribute("maxInactivityTimeMs").as_int(60000);
  
    _certFile = _doc.child(configRoot).child(sslNode).attribute("certfile").as_string("");
    _keyFile = _doc.child(configRoot).child(sslNode).attribute("keyfile").as_string("");
    
}

int Config::getServerThreadsNumber() 
{
    return _threadNumber;
}

int Config::getServerPort() 
{
    return _serverPort;
}


int Config::getAdaServerPort() 
{
    return _adaServerPort;
}

int Config::getNotifyPort() 
{
    return 0;
}

int Config::getAlgorithmPort() 
{
    return _algorithmPort;
}

int Config::getGamificationPort() 
{
    return _gamificationPort;
}


int Config::getVerbosity() 
{
    return _verbosity;
}

string Config::getDBConnectionString() 
{
    return _connectionString;
}

int Config::getDBSessionsNumber() 
{
    return _sessionsNumber;
}

int Config::getComTableSleepPeriodMs() 
{
    return _comTableSleepPeriodMs;
  }

int Config::getComTableMaxInactivityMs() 
{
    return _comTableMaxInactivityMs;  
}

bool Config::isLogsPrintedToCout() 
{
    return _logsToCout;
}

std::string Config::getLogsFolder() 
{
    return _logsFolder;
}

std::string Config::getCertFile() 
{
    return _certFile;
}

std::string Config::getKeyFile() 
{
    return _keyFile;
}

bool Config::isDebugMode() 
{
    return _debugMode;
}
