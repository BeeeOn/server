/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WatchdogManager.h
 * Author: mrmaidx
 *
 * Created on 21. ledna 2016, 14:39
 */

#ifndef WATCHDOGMANAGER_H
#define WATCHDOGMANAGER_H

#include <string>

#include "../src/EventAlgorithmManager.h"



class WatchdogManager : public EventAlgorithmManager 
{
public:
    
    
    
    WatchdogManager();
    WatchdogManager(const WatchdogManager& orig);
    virtual ~WatchdogManager();
    
    void processDataMessage();
private:

};

#endif /* WATCHDOGMANAGER_H */

