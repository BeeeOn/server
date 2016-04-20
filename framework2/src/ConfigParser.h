/* 
 * File:   ConfigParser.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 18. March 2016
 */

#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H



#include "pugixml.hpp"
#include "pugiconfig.hpp"

class ConfigParser {
public:
    /**
     * Constructor of class ConfigParser.
     */
    ConfigParser();
    /**
     * Destructor of class ConfigParser.
     */
    virtual ~ConfigParser();
    /**
     * Parses config file and stores data in static Config object.
     * @param config_file_path Path to config file.
     */
    void parseConfigFile(char *config_file_path);
};

#endif /* CONFIGPARSER_H */
