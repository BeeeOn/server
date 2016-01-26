/* 
 * File:   TestInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 26. January 2016
 */

#ifndef TESTINSTANCE_H
#define TESTINSTANCE_H

#include <string>
#include <memory>

#include "../src/TimedAlgorithmInstance.h"

class TestInstance: public TimedAlgorithmInstance
{
public:
    TestInstance(unsigned long user_id, unsigned int users_instance_personal_id);
    
    virtual ~TestInstance();
    
    void run();
private:
    const std::string m_text_to_write = "<<<Test algorithm text>>.";
    
    int m_activated_times = 1;
};

#endif /* TESTINSTANCE_H */