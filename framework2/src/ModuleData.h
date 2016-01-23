/* 
 * File:   ModuleData.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
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

