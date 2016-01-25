/* 
 * File:   TimedAlgorithmInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 22. January 2016
 */

#ifndef TIMEDALGORITHMINSTANCE_H
#define TIMEDALGORITHMINSTANCE_H

#include <memory>
#include <string>

#include "AlgorithmInstance.h"


class TimedAlgorithmInstance: public AlgorithmInstance//, enable_shared_from_this<TimedAlgorithmInstance>
{
public:
    /*std::shared_ptr<TimedAlgorithmInstance> getptr() {
        return shared_from_this();
    }
    */
    
    TimedAlgorithmInstance(unsigned int manager_id, unsigned long instance_id, unsigned long user_id, unsigned int users_instance_personal_id, std::string text);
    
    //TimedAlgorithmInstance(const TimedAlgorithmInstance& orig);
    //virtual ~TimedAlgorithmInstance();

    //void emplaceToCalendar();
    
    // Plans to calendar when shoud be run again.
    void planActivationAfterSeconds(int seconds);
    
    // Actualy runs code.
    virtual void activate();
    
private:

};

#endif /* TIMEDALGORITHMINSTANCE_H */

