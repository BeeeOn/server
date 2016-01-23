/* 
 * File:   TimedAlgorithmManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef TIMEDALGORITHMMANAGER_H
#define TIMEDALGORITHMMANAGER_H

#include "AlgorithmManager.h"

class TimedAlgorithmManager : public AlgorithmManager
{
public:
    
    TimedAlgorithmManager(int id, MANAGER_TYPE type, std::string name);
    
    //TimedAlgorithmManager(const TimedAlgorithmManager& orig);
    
    virtual ~TimedAlgorithmManager();
private:

};

#endif /* TIMEDALGORITHMMANAGER_H */

