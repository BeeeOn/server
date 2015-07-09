/* 
 * File:   Config.h
 * Author: pavel
 *
 * Created on 18. červenec 2014, 15:10
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include "../lib/pugixml.hpp"

#include <string>
#include <iostream>
class Config {
private:
    Config();
    Config(Config const&);              // Don't Implement
    void operator=(Config const&); // Don't implement
public:
    virtual ~Config();
    static Config& getInstance();
    void loadXml(std::string file);
    
    int getServerThreadsNumber();
    int getServerPort();
    
    int getActivityPort();
    int getNotifyPort();
    int getAlgorithmPort();
    int getGamificationPort();
    int getVerbosity();
    
    std::string getDBConnectionString();
    int getDBSessionsNumber();
    
    int getComTableSleepPeriodMs();
    int getComTableMaxInactivityMs();
    
    bool isLogsPrintedToCout();
    std::string getLogsFolder();
    
private:
    pugi::xml_document _doc;
    
    int _threadNumber;    
    int _serverPort;
    
    int _activityPort;
    int _notifyPort;
    int _algorithmPort;
    int _gamificationPort;
    int _verbosity;
    
    std::string _connectionString;
    int _sessionsNumber;
    
    bool _logsToCout;
    std::string _logsFolder;
};

#endif	/* CONFIG_H */

