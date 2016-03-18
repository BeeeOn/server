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
    
    /** Path to file containing configuration of tasks.*/
    std::string m_tasks_config_path;
    
    /** Number of working threads user server is going to have. */
    int m_user_server_threads;
    
    /** Port assigned to user server. */
    unsigned short m_user_server_port;
    
    /** Number of working threads gateway server is going to have. */
    int m_gateway_server_threads;
    
    /** Port assigned to gateway server. */
    unsigned short m_gateway_server_port;
};

#endif /* CONFIGPARSER_H */

