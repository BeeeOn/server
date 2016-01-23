/* 
 * File:   ConfigMessage.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 22. January 2016
 */

#ifndef CONFIGMESSAGE_H
#define CONFIGMESSAGE_H

#include <map>
#include <string>

class ConfigMessage {
public:
    ConfigMessage();
    ConfigMessage(const ConfigMessage& orig);
    virtual ~ConfigMessage();
private:
    
    int manager_id;
    
    std::map<std::string /*tag_name*/, std::string /*value*/> configuration;
    
};

#endif /* CONFIGMESSAGE_H */

