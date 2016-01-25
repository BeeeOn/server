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

#include "../src/TimedAlgorithmInstance.h"

class RefreshInstance: public TimedAlgorithmInstance
{
public:
   
    /*
    std::shared_ptr<RefreshInstance> getptr() {
        return shared_from_this();
    }
    */
    RefreshInstance(unsigned int manager_id, unsigned long instance_id, unsigned long user_id, unsigned int users_instance_personal_id, std::string text);
    
    //RefreshInstance(const RefreshInstance& orig);
    
    //void planActivationAfterSeconds(int seconds);
    
    virtual ~RefreshInstance();
    
    void activate();

private:
    int m_activated_times = 0;

};
#endif /* REFRESHINSTANCE_H */

