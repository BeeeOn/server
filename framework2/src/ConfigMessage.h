/* 
 * File:   ConfigMessage.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 23. March 2016
 */

#ifndef CONFIGMESSAGE_H
#define CONFIGMESSAGE_H

#include <map> //std::map
#include <string> //std::string

struct ConfigMessage {
    
    unsigned int user_id;
    short task_id;
    unsigned short relative_id;
    std::map<std::string /*name*/, std::string /*value*/> parameters;
};

#endif /* CONFIGMESSAGE_H */

