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

void Config::loadXml(string file) {
    pugi::xml_parse_result result = _doc.load_file(file.c_str());
    if(!result){
        string desc = result.description();
        throw "Cant load config XML file:"+file+" Error description: " + desc + "\n"; 
    }
}

int Config::getServerThreadsNumber() {
    return _doc.child(CONFIG_ROOT).child(SERVER_NODE).attribute("threads").as_int(1);
}

int Config::getServerPort() {
    return _doc.child(CONFIG_ROOT).child(SERVER_NODE).attribute("port").as_int();
}


int Config::getActivityPort() {
    return _doc.child(CONFIG_ROOT).child(SERVER_NODE).attribute("activityPort").as_int();
}

int Config::getNotifyPort() {
    return _doc.child(CONFIG_ROOT).child(SERVER_NODE).attribute("notifyPort").as_int();
}

int Config::getVerbosity() {
    return _doc.child(CONFIG_ROOT).child(SERVER_NODE).attribute("verbosity").as_int(10);//10 = max verbosity
}


string Config::getDBConnectionString() {
    return _doc.child(CONFIG_ROOT).child(DB_NODE).attribute("connectionString").as_string(DEFAULT_DB_CONNECTION_STRING);
}

int Config::getDBSessionsNumber() {
    return _doc.child(CONFIG_ROOT).child(DB_NODE).attribute("sessions").as_int(10);
}

int Config::getComTableSleepPeriodMs() {
    return _doc.child(CONFIG_ROOT).child(COMTABLE_NODE).attribute("cleanerSleepPeriodMs").as_int(1000);
}

int Config::getComTableMaxInactivityMs() {
    return _doc.child(CONFIG_ROOT).child(COMTABLE_NODE).attribute("maxInactivityTimeMs").as_int(60000);
}

