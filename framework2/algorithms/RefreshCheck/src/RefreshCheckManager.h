/* 
 * File:   RefreshManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#ifndef REFRESHMANAGER_H
#define REFRESHMANAGER_H

#include <iostream>
#include <string>

#include "../../../src/TaskManager.h"

class RefreshCheckManager: public TaskManager
{
public:
    
    RefreshCheckManager();
    
    void createInstance(unsigned int user_id, unsigned short personal_id);
    
    void deleteInstance();

    //RefreshManager(const RefreshManager& orig);
    //void createInstance(unsigned long user_id, unsigned int users_instance_personal_id);
    
    virtual ~RefreshCheckManager();
private:

};
#endif /* REFRESHMANAGER_H */

