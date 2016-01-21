/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ModuleData.h
 * Author: mrmaidx
 *
 * Created on 21. ledna 2016, 20:50
 */

#ifndef MODULEDATA_H
#define MODULEDATA_H

// Object holding data from a module.
class ModuleData {
public:
    ModuleData();
    ModuleData(const ModuleData& orig);
    virtual ~ModuleData();
    
    int gateway_id;
    int device_euid;
    int device_id;
    int module_id;
    float value;
    
private:

};

#endif /* MODULEDATA_H */

