/* 
 * File:   ManagerLoader.h
 * Author: mrmaidx
 *
 * Created on 20. ledna 2016, 19:22
 */

#ifndef MANAGERLOADER_H
#define MANAGERLOADER_H

#include <map>

#include "AlgorithmManager.h"

class ManagerLoader {
public:
    ManagerLoader();
    ManagerLoader(const ManagerLoader& orig);
    virtual ~ManagerLoader();
    
    void loadAlgorithms();
    
private:
    std::map<int /*manager_id*/, AlgorithmManager> algorithm_managers;
    
};

#endif /* MANAGERLOADER_H */

