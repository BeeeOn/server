/* 
 * File:   Config.cpp
 * Author: pavel
 * 
 * Created on 18. červenec 2014, 15:10
 */

#include "Config.h"

#define CONFIG_ROOT "config"
#define SERVER_NODE "server"
#define DB_NODE "database"
#define COMTABLE_NODE "comTable"
#define LOGS_NODE "logs"

using namespace std;

#define DEFAULT_DB_CONNECTION_STRING ""

Config::Config() {
    loadXml("config.xml");
}

Config::~Config() {
}


Config& Config::getInstance()
{
    static Config instance;
    return instance;
}

void Config::loadXml(std::string file) {
    pugi::xml_parse_result result = _doc.load_file(file.c_str());
    if(!result){
        std::string desc = result.description();
        throw "Cant load config XML file:"+file+" Error description: " + desc + "\n"; 
    }
    _threadNumber = _doc.child(CONFIG_ROOT).child(SERVER_NODE).attribute("threads").as_int(1);
    _serverPort = _doc.child(CONFIG_ROOT).child(SERVER_NODE).attribute("port").as_int();
    
    _activityPort = _doc.child(CONFIG_ROOT).child(SERVER_NODE).attribute("activityPort").as_int();
    _algorithmPort = _doc.child(CONFIG_ROOT).child(SERVER_NODE).attribute("algorithmPort").as_int();
    _gamificationPort = _doc.child(CONFIG_ROOT).child(SERVER_NODE).attribute("gamificationPort").as_int();
    _verbosity = _doc.child(CONFIG_ROOT).child(SERVER_NODE).attribute("verbosity").as_int(10);//10 = max verbosity
    
    _connectionString = _doc.child(CONFIG_ROOT).child(DB_NODE).attribute("connectionString").as_string(DEFAULT_DB_CONNECTION_STRING);
    _sessionsNumber = _doc.child(CONFIG_ROOT).child(DB_NODE).attribute("sessions").as_int(10);
    
    _logsToCout = _doc.child(CONFIG_ROOT).child(LOGS_NODE).attribute("toCout").as_bool(true);
    _logsFolder = _doc.child(CONFIG_ROOT).child(LOGS_NODE).attribute("folder").as_string("logs");
}

int Config::getServerThreadsNumber() {
    return _threadNumber;
}

int Config::getServerPort() {
    return _serverPort;
}


int Config::getActivityPort() {
    return _activityPort;
}

int Config::getNotifyPort() {
    return 0;
}

int Config::getAlgorithmPort() {
    return _algorithmPort;
}

int Config::getGamificationPort() {
    return _gamificationPort;
}


int Config::getVerbosity() {
    return _verbosity;
}

string Config::getDBConnectionString() {
    return _connectionString;
}

int Config::getDBSessionsNumber() {
    return _sessionsNumber;
}

int Config::getComTableSleepPeriodMs() {
    return _doc.child(CONFIG_ROOT).child(COMTABLE_NODE).attribute("cleanerSleepPeriodMs").as_int(1000);
}

int Config::getComTableMaxInactivityMs() {
    return _doc.child(CONFIG_ROOT).child(COMTABLE_NODE).attribute("maxInactivityTimeMs").as_int(60000);
}

bool Config::isLogsPrintedToCout() {
    return _logsToCout;
}

std::string Config::getLogsFolder() {
    return _logsFolder;
}

