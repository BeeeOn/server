#ifndef CONFIG_H
#define	CONFIG_H

#include "../lib/pugixml.hpp"

#include <string>
#include <iostream>
class Config {
public:
    Config();
    virtual ~Config();
    static Config& getInstance();
    void loadXml(std::string file);
    
    int getServerThreadsNumber();
    int getServerPort();
    
    int getAdaServerPort();
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
    
    std::string getCertFile();
    std::string getKeyFile();
    
    bool isDebugMode();
    
private:
    pugi::xml_document _doc;
    
    int _threadNumber;    
    int _serverPort;
    
    int _adaServerPort;
    int _notifyPort;
    int _algorithmPort;
    int _gamificationPort;
    int _verbosity;
    
    std::string _certFile;
    std::string _keyFile;
    
    std::string _connectionString;
    int _sessionsNumber;
    
    int _comTableSleepPeriodMs;
    int _comTableMaxInactivityMs;
    
    
    bool _logsToCout;
    std::string _logsFolder;
    
    bool _debugMode;
};

#endif	/* CONFIG_H */

