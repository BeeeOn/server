/* 
 * File:   AlgorithmInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef ALGORITHMINSTANCE_H
#define ALGORITHMINSTANCE_H

#include <map>
#include <string>

class AlgorithmInstance {
public:
    AlgorithmInstance(unsigned int manager_id, unsigned long instance_id, unsigned long user_id, unsigned int users_instance_personal_id, std::string text);
    
    //AlgorithmInstance(const AlgorithmInstance& orig);
    
    //virtual ~AlgorithmInstance();
    
protected:
    
    unsigned int m_manager_id;
    // Unique id for instance in a manager.
    unsigned long m_instance_id;

    std::string m_text;
    
    // There can be multiple owners of one instance.
    // First is user_id from database, second is users personal users_instance_personal_id
    // (example: user can have a several running instances of one algorithm).
    // THERE SHOULD BE ROLES FOR INSTANCES (OWNER? VIEWER?).
    std::map<unsigned long /*user_id*/, unsigned int /*users_instance_personal_id*/> m_owners;
};

#endif /* ALGORITHMINSTANCE_H */