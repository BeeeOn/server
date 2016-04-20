/* 
 * File:   ConfigParser.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 18. March 2016
 */

#include "ConfigParser.h"

#include <stdexcept>

#include "Config.h"
#include "Logger.h"

// Definition of static member variables of struct Config.
std::string Config::m_tasks_config_path;
int Config::m_user_server_threads;
int Config::m_user_server_port;
int Config::m_gateway_server_threads;
int Config::m_gateway_server_port;
int Config::m_database_sessions;
std::string Config::m_database_connection_string;
std::string Config::m_log_folder_path;
std::string Config::m_log_level;
int Config::m_ada_server_sender_port; 

ConfigParser::ConfigParser()
{
}

ConfigParser::~ConfigParser()
{
}

void ConfigParser::parseConfigFile(char *config_file_path)
{
    pugi::xml_node current_node;
    pugi::xml_attribute current_attribute;
    
    pugi::xml_document config_doc;
    
    // Parse config file.
    pugi::xml_parse_result result = config_doc.load_file(config_file_path);

    if (!result) {
        logger.LOGFILE("config_parser", "FATAL") << "Could not open baf config file: "
                << config_file_path << ". Error description: " << result.description()
                << ", Error offset: " << result.offset << " (error at [..."
                << (config_file_path + result.offset) << "]" << std::endl;
        
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    // Find <config> tag.
    current_node = config_doc.child("config");
    if (current_node == NULL) {
        logger.LOGFILE("config_parser", "FATAL") << "Config file doesn't contain"
                << "root tag <config>." << std::endl;
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    
    // Find and parse <tasks_config> tag. 
    current_attribute = current_node.child("tasks_config").attribute("path");
    if (current_attribute == NULL) {
        logger.LOGFILE("config_parser", "FATAL") << "Config file doesn't contain"
                << "tag <tasks_config path=\"...\"/> in correct form." << std::endl;
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    else {
        // Parse attribute as string.
        Config::m_tasks_config_path = current_attribute.as_string();
    }

    // Find and parse <user_server> tag.
    current_node = current_node.child("user_server");
    if ((current_node.attribute("threads") == NULL) ||
        (current_node.attribute("port") == NULL)) {
        logger.LOGFILE("config_parser", "FATAL") << "Config file doesn't contain"
                << "tag <user_server threads=\"...\" port=\"...\"/> in correct form." << std::endl;
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    else {
        Config::m_user_server_port = current_node.attribute("port").as_int();
        Config::m_user_server_threads = current_node.attribute("threads").as_int();
    }

    // Find and parse <gateway_server> tag.
    current_node = current_node.parent().child("gateway_server");
    if ((current_node.attribute("threads") == NULL) ||
        (current_node.attribute("port") == NULL)) {
        logger.LOGFILE("config_parser", "FATAL") << "Config file doesn't contain"
                << "tag <gateway_server threads=\"...\" port=\"...\"/> in correct form." << std::endl;
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    else {
        Config::m_gateway_server_port = current_node.attribute("port").as_int();
        Config::m_gateway_server_threads = current_node.attribute("threads").as_int();
    }

    // Find and parse <database> tag.
    current_node = current_node.parent().child("database");
    if ((current_node.attribute("sessions") == NULL) ||
        (current_node.attribute("connection_string") == NULL)) {
        logger.LOGFILE("config_parser", "FATAL") << "Config file doesn't contain"
                << "tag <database sessions=\"...\" connection_string=\"...\"/> in correct form." << std::endl;
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    else {
        Config::m_database_sessions = current_node.attribute("sessions").as_int();
        Config::m_database_connection_string = current_node.attribute("connection_string").as_string();
    }

    // Find and parse <logger> tag.
    current_node = current_node.parent().child("logger");
    if ((current_node.attribute("log_folder_path") == NULL) ||
        (current_node.attribute("log_level") == NULL)) {
        logger.LOGFILE("config_parser", "FATAL") << "Config file doesn't contain"
                << "tag <logger log_folder_path=\"...\" log_level=\"...\"/> in correct form." << std::endl;
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    else {
        Config::m_log_folder_path = current_node.attribute("log_folder_path").as_string();
        Config::m_log_level = current_node.attribute("log_level").as_string();
    }

    // Find and parse <tasks_config> tag. 
    current_attribute = current_node.parent().child("ada_server_sender").attribute("port");
    if (current_attribute == NULL) {
        logger.LOGFILE("config_parser", "FATAL") << "Config file doesn't contain"
                << "tag <ada_server_sender port=\"...\"/> in correct form." << std::endl;
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    else {
        // Parse attribute as string.
        Config::m_ada_server_sender_port = current_attribute.as_int();
    }
}
