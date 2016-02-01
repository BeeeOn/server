/* 
 * File:   TimedAlgorithmInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 22. January 2016
 */

#ifndef TIMEDALGORITHMINSTANCE_H
#define TIMEDALGORITHMINSTANCE_H

#include <memory>
#include <mutex>
#include <string>

#include "AlgorithmInstance.h"


class TimedAlgorithmInstance: public AlgorithmInstance//, enable_shared_from_this<TimedAlgorithmInstance>
{
public:
    /*std::shared_ptr<TimedAlgorithmInstance> getptr() {
        return shared_from_this();
    }
    */
    
    TimedAlgorithmInstance(unsigned long user_id, unsigned int users_instance_personal_id);
    
    //TimedAlgorithmInstance(const TimedAlgorithmInstance& orig);
    virtual ~TimedAlgorithmInstance();

    //void emplaceToCalendar();
    
    // Plans to calendar when shoud be run again.
    void planActivationAfterSeconds(int seconds);
    
    void planActivationNow();
    
    // Actualy runs code.
    void activate();
    
    virtual void run();
    
private:
    std::mutex m_activation_mx;

};

#endif /* TIMEDALGORITHMINSTANCE_H */

