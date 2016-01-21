/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AlgorithmManager.h
 * Author: mrmaidx
 *
 * Created on 20. ledna 2016, 16:09
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
    AlgorithmManager();
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

