/* 
 * File:   AlgorithmManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef ALGORITHMMANAGER_H
#define ALGORITHMMANAGER_H

#include <map>
#include <memory> //std::shared_ptr
//#include <utility> // std::pair

#include "AlgorithmInstance.h"

enum class MANAGER_TYPE {
    EVENT, TIMED
};

class AlgorithmManager {
public:
    
    AlgorithmManager(unsigned int id, MANAGER_TYPE type, std::string name);
    
    AlgorithmManager(const AlgorithmManager& orig);
    virtual ~AlgorithmManager();
        
    void processConfigMessage();
    
    // Getters.
    unsigned int getId() const {return m_id;}
    std::string getName() const {return m_name;};
    MANAGER_TYPE getType() const {return m_type;};    
    
    // Setters.
    void setType(MANAGER_TYPE type) {m_type = type;}
    
    // Creates an instance of algorithm.
    //virtual void createInstance();
    
protected:
    // Variable for generating unique ids for instances.  
    unsigned long m_instance_id_counter = 1;
    
    // Id of manager.
    unsigned int m_id;
    
    std::string m_name;
    
    MANAGER_TYPE m_type;
    

};

#endif /* ALGORITHMMANAGER_H */