/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UserMessageParser.cpp
 * Author: mrmaidx
 * 
 * Created on 20. března 2016, 22:36
 */

#include "UserMessageParser.h"

UserMessageParser::UserMessageParser() {
}

UserMessageParser::~UserMessageParser() {
}

void UserMessageParser::parseMessage(std::string message) {
    
    rapidjson::ParseResult result = m_document.Parse(message.c_str());
    if (!result) {
        std::cerr << "UserMessageParser: JSON parsing error." /*<< rapidjson::GetParseError(result.Code()) << result.Offset()*/ << std::endl;
        throw std::runtime_error("UserMessageParser: Processing of received message was not successful.");
    }
    
    if(!m_document.IsObject()) {
        throw std::runtime_error("UserMessageParser: Received JSON message is not valid object.");
    }
    
    if (m_document.HasMember("baf")) {
        rapidjson::Value& baf_value = m_document["baf"];
        
        if (!baf_value.IsString()) {
            throw std::runtime_error("UserMessageParser: Received JSON message has member \"baf\", but it's not a string.");
        }
        else {
            std::string string_type = baf_value.GetString();
        
            // Store type of received message.
            if (string_type == "config") {
                m_message_type = USER_MESSAGE_TYPE::CONFIG;
            }
        }
    }
    else {
        throw std::runtime_error("UserMessageParser: Received JSON message doesn't have member \"baf\". It's not a message meant for BAF.");
    }
}

ConfigMessage UserMessageParser::getConfigData() {
    
    ConfigMessage config_message;
    
    if (m_document.HasMember("user_id")) {
        rapidjson::Value& user_id_value = m_document["user_id"];
        
        if (!user_id_value.IsInt()) {
            throw std::runtime_error("UserMessageParser: Received CONFIG message has member \"user_id\", but it's not an integer.");
        }
        else {
            // Store user_id.
            config_message.user_id = user_id_value.GetInt();
        }
    }
    else {
        throw std::runtime_error("UserMessageParser: Received CONFIG message doesn't have member \"user_id\".");
    }
    
    if (m_document.HasMember("task_id")) {
        rapidjson::Value& task_id_value = m_document["task_id"];
        
        if (!task_id_value.IsInt()) {
            throw std::runtime_error("UserMessageParser: Received CONFIG message has member \"task_id\", but it's not an integer.");
        }
        else {
            // Store task_id.
            config_message.task_id = task_id_value.GetInt();
        }
    }
    else {
        throw std::runtime_error("UserMessageParser: Received CONFIG message doesn't have member \"task_id\".");
    }
    
    if (m_document.HasMember("relative_id")) {
        rapidjson::Value& relative_id_value = m_document["relative_id"];
        
        if (!relative_id_value.IsInt()) {
            throw std::runtime_error("UserMessageParser: Received CONFIG message has member \"relative_id\", but it's not an integer.");
        }
        else {
            // Store task_id.
            config_message.relative_id = relative_id_value.GetInt();
        }
    }
    else {
        throw std::runtime_error("UserMessageParser: Received CONFIG message doesn't have member \"relative_id\".");
    }    
    
    
    if (m_document.HasMember("parameters")) {
        rapidjson::Value& parameters_value = m_document["parameters"];
        
        if (!parameters_value.IsObject()) {
            throw std::runtime_error("UserMessageParser: Received CONFIG message has member \"parameters\", but it's not an object.");
        }
        else {
            for (rapidjson::Value::ConstMemberIterator itr = parameters_value.MemberBegin(); itr != parameters_value.MemberEnd(); itr++)
            {
                std::string name = itr->name.GetString();
                    
                if (!(itr->value.IsString())) {
                    
                    throw std::runtime_error(std::string("UserMessageParser: Received CONFIG message has entry: ") + name + std::string(" which is not string type." ));
                }
                else {
                    
                    config_message.parameters.emplace(name, std::string(itr->value.GetString()));
                }
            }
        }
    }
    else {
        throw std::runtime_error("UserMessageParser: Received CONFIG message doesn't have member object \"parameters\".");
    }
    
    return config_message;
}