/* 
 * File:   ManagerLoader.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef MANAGERLOADER_H
#define MANAGERLOADER_H

#include <map>
#include <memory>

#include "EventAlgorithmManager.h"
#include "TimedAlgorithmManager.h"

class ManagerLoader {
public:
    ManagerLoader();
    ManagerLoader(const ManagerLoader& orig);
    virtual ~ManagerLoader();
    
    // Loads algorithm 
    void loadAlgorithmManagers();
    
    // Sends data message to the right event algorithm manager.
    void passDataMessage();
    
    void passConfigMessage();
    
    static void activateInstance(int manager_id, unsigned long instance_id);
    
private:
    //std::map<int /*manager_id*/, AlgorithmManager> algorithm_managers;
    
    static std::map<int /*manager_id*/, std::shared_ptr<TimedAlgorithmManager>> m_timed_managers;
    
    std::map<int /*manager_id*/, EventAlgorithmManager> event_managers;
    
};

#endif /* MANAGERLOADER_H */

