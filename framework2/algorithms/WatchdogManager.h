/* 
 * File:   WatchdogManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#ifndef WATCHDOGMANAGER_H
#define WATCHDOGMANAGER_H

#include <string>

#include "../src/EventAlgorithmManager.h"

class WatchdogManager : public EventAlgorithmManager 
{
public:

    WatchdogManager(int id, MANAGER_TYPE type, std::string name);
    //WatchdogManager(const WatchdogManager& orig);
    virtual ~WatchdogManager();
    
    void processDataMessage();
private:

};

#endif /* WATCHDOGMANAGER_H */

