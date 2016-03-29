/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataMessage.h
 * Author: mrmaidx
 *
 * Created on 28. března 2016, 14:10
 */

#ifndef DATAMESSAGE_H
#define DATAMESSAGE_H

#include <map>

struct DataMessage {
    unsigned long long gateway_id;
    unsigned long long device_euid;
    unsigned int time;
    std::map<int /*module_id*/, double /*value*/> modules;
};


#endif /* DATAMESSAGE_H */

