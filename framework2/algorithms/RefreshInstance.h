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
    RefreshInstance(unsigned long user_id, unsigned int users_instance_personal_id);
    
    virtual ~RefreshInstance();
    
    void run();
    
private:
    const std::string m_text_to_write = "Custom message stored in RefreshInstance.";
    
    int m_activated_times = 1;

    void refreshCustomFunction();
};
#endif /* REFRESHINSTANCE_H */

