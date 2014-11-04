/* 
 * File:   Config.h
 * Author: pavel
 *
 * Created on 18. červenec 2014, 15:10
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include "pugixml.hpp"

using namespace std;

class Config {
private:
    Config();
    Config(Config const&);              // Don't Implement
    void operator=(Config const&); // Don't implement
public:
    virtual ~Config();
    static Config& getInstance();
    void loadXml(string file);
    
    int getServerThreadsNumber();
    int getServerPort();
    
    int getActivityPort();
    int getNotifyPort();
    int getVerbosity();
    
    string getDBConnectionString();
    int getDBSessionsNumber();
    
    int getComTableSleepPeriodMs();
    int getComTableMaxInactivityMs();
    
    
private:
    pugi::xml_document _doc;


/* connection string
 pocet vlaken
 buzeni threadu na clean com table
 * minuty neaktivity do resign
 */    
};

#endif	/* CONFIG_H */

