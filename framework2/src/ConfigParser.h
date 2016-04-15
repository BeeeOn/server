/* 
 * File:   ConfigParser.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 18. March 2016
 */

#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <stdexcept>

#include "pugixml.hpp"
#include "pugiconfig.hpp"

class ConfigParser {
public:
    ConfigParser();
    
    virtual ~ConfigParser();
    
    /**
     * Parses config file and stores data to member variables.
     * @param config_file_path Path to config file.
     */
    void parseConfigFile(char *config_file_path);
};

#endif /* CONFIGPARSER_H */

