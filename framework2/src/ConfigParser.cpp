/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConfigParser.cpp
 * Author: mrmaidx
 * 
 * Created on 18. března 2016, 15:37
 */

#include "ConfigParser.h"

#include <iostream>

ConfigParser::ConfigParser() {
}

ConfigParser::~ConfigParser() {
}

void ConfigParser::parseConfigFile(char *config_file_path) {

    pugi::xml_node current_node;
    pugi::xml_attribute current_attribute;
    
    pugi::xml_document config_doc;
    
    pugi::xml_parse_result result = config_doc.load_file(config_file_path);

    if (!result) {
        std::cerr << "Could not open baf config file: " << config_file_path << std::endl;
        std::cerr << "Error description: " << result.description() << std::endl;
        std::cerr << "Error offset: " << result.offset << " (error at [..." << (config_file_path + result.offset) << "]" << std::endl;
        
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    /*
     Format of config file:
     <config>   
        <tasks_config path="..."/>
        <user_server threads="..." port="..." />
        <gateway_server threads="..." port="..." />
     </config>
    */
    
    // Find <config> tag.
    current_node = config_doc.child("config");
    if (current_node == NULL) {
        std::cerr << "Config file doesn't contain root tag <config>." << std::endl;
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    
    // Find and parse <tasks_config> tag. 
    current_attribute = current_node.child("tasks_config").attribute("path");
    if (current_attribute == NULL) {
        std::cerr << "Config file doesn't contain tag <tasks_config path=\"...\"/> in correct form." << std::endl;
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    else {
        // Parse attribute as string.
        m_tasks_config_path = current_attribute.as_string();
    }
    
    // Find and parse <user_server> tag.
    current_node = current_node.child("user_server");
    if ((current_node.attribute("threads") == NULL) ||
        (current_node.attribute("port") == NULL)) {
        std::cerr << "Config file doesn't contain tag <user_server threads=\"...\" port=\"...\"/> in correct form." << std::endl;
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    else {
        m_user_server_port = current_node.attribute("threads").as_int();
        m_user_server_threads = current_node.attribute("port").as_int();
    }
    
    // Find and parse <gateway_server> tag.
    current_node = current_node.parent().child("gateway_server");
    if ((current_node.attribute("threads") == NULL) ||
        (current_node.attribute("port") == NULL)) {
        std::cerr << "Config file doesn't contain tag <gateway_server threads=\"...\" port=\"...\"/> in correct form." << std::endl;
        throw std::runtime_error("Parsing of config file was not successful.");
    }
    else {
        m_gateway_server_port = current_node.attribute("threads").as_int();
        m_gateway_server_threads = current_node.attribute("port").as_int();
    }
}
