/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EventAlgorithmManager.h
 * Author: mrmaidx
 *
 * Created on 20. ledna 2016, 16:42
 */

#ifndef EVENTALGORITHMMANAGER_H
#define EVENTALGORITHMMANAGER_H

#include "AlgorithmManager.h"

class EventAlgorithmManager : public AlgorithmManager {
public:
    
    
    EventAlgorithmManager();
    EventAlgorithmManager(const EventAlgorithmManager& orig);
    virtual ~EventAlgorithmManager();
    
    
    virtual void processDataMessage();
    

private:

};

#endif /* EVENTALGORITHMMANAGER_H */

