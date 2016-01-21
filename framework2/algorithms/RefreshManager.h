/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RefreshManager.h
 * Author: mrmaidx
 *
 * Created on 21. ledna 2016, 14:40
 */

#ifndef REFRESHMANAGER_H
#define REFRESHMANAGER_H

#include "../src/TimedAlgorithmManager.h"

class RefreshManager : public TimedAlgorithmManager
{
public:
    RefreshManager();
    RefreshManager(const RefreshManager& orig);
    virtual ~RefreshManager();
private:

};

#endif /* REFRESHMANAGER_H */

