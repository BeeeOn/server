/* 
 * File:   EventAlgorithmManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef EVENTALGORITHMMANAGER_H
#define EVENTALGORITHMMANAGER_H

#include "AlgorithmManager.h"

class EventAlgorithmManager : public AlgorithmManager
{
public:
    
    EventAlgorithmManager(MANAGER_TYPE type, std::string name);
    
    //EventAlgorithmManager(const EventAlgorithmManager& orig);
    
    virtual ~EventAlgorithmManager();
    
    virtual void processDataMessage();
    
private:

};

#endif /* EVENTALGORITHMMANAGER_H */

