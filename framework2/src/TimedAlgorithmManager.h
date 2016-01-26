/* 
 * File:   TimedAlgorithmManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef TIMEDALGORITHMMANAGER_H
#define TIMEDALGORITHMMANAGER_H

#include <map>
#include <memory>
#include <string>

#include "AlgorithmManager.h"
#include "TimedAlgorithmInstance.h"

class TimedAlgorithmManager : public AlgorithmManager
{
public:
    
    TimedAlgorithmManager(MANAGER_TYPE type, std::string name);
    
    //TimedAlgorithmManager(const TimedAlgorithmManager& orig);
    
    virtual ~TimedAlgorithmManager();
    
    // Creates an instance of algorithm.
    virtual void createInstance(unsigned long user_id, unsigned int users_instance_personal_id);
    
protected:

    // Map containing instance_id as a key, and pointer to an object of an instance of an algorithm.
    std::map<unsigned long /*instance_id*/, std::shared_ptr<TimedAlgorithmInstance>> m_algorithm_instances;
};

#endif /* TIMEDALGORITHMMANAGER_H */

