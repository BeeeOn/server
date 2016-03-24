/* 
 * File:   RefreshInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#ifndef REFRESHINSTANCE_H
#define REFRESHINSTANCE_H

#include <string>
#include <memory>

#include "../../../src/TimedTaskInstance.h"

class RefreshCheckInstance: public TimedTaskInstance
{
public:
    RefreshCheckInstance(unsigned long user_id, unsigned int users_instance_personal_id);
    
    virtual ~RefreshCheckInstance();
    
    void run();
    
private:

    void executeRefreshQuery();
};
#endif /* REFRESHINSTANCE_H */

