/* 
 * File:   AlgorithmManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef ALGORITHMMANAGER_H
#define ALGORITHMMANAGER_H

#include <map>
#include <utility> // std::pair

#include "AlgorithmInstance.h"

enum class MANAGER_TYPE {
    EVENT, TIMED
};

class AlgorithmManager {
public:
    
    AlgorithmManager(int id, MANAGER_TYPE type, std::string name);
    
    AlgorithmManager(const AlgorithmManager& orig);
    virtual ~AlgorithmManager();
    
    void processConfigMessage();
    
    // Getters.
    int getId() const {return m_id;}
    std::string getName() const {return m_name;};
    MANAGER_TYPE getType() const {return m_type;};    
    
    void setType(MANAGER_TYPE type) {
        m_type = type;
    }
    
protected:
    // Id of manager.
    int m_id;
    
    std::string m_name;
    
    MANAGER_TYPE m_type;
    
    // Map containing user_id and instance_id as a key, and object of instance of algorithm.
    std::map<std::pair<int /*user_id*/, int /*instance_id*/>, AlgorithmInstance /**/> algorithm_instances; 
};

#endif /* ALGORITHMMANAGER_H */

