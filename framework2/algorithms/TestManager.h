/* 
 * File:   TestManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 26. January 2016
 */

#ifndef TESTMANAGER_H
#define TESTMANAGER_H

#include <string>

#include "../src/TimedTaskManager.h"

class TestManager: public TimedTaskManager
{
public:
    TestManager(MANAGER_TYPE type, std::string name);
    
    virtual ~TestManager();
    
    void createInstance(unsigned long user_id, unsigned int users_instance_personal_id);
};

#endif /* TESTMANAGER_H */