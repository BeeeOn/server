/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TimedAlgorithmManager.h
 * Author: mrmaidx
 *
 * Created on 20. ledna 2016, 16:42
 */

#ifndef TIMEDALGORITHMMANAGER_H
#define TIMEDALGORITHMMANAGER_H

#include "AlgorithmManager.h"

class TimedAlgorithmManager : public AlgorithmManager
{
public:
    TimedAlgorithmManager();
    TimedAlgorithmManager(const TimedAlgorithmManager& orig);
    virtual ~TimedAlgorithmManager();
private:

};

#endif /* TIMEDALGORITHMMANAGER_H */

