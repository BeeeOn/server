/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UserMessageParser.h
 * Author: mrmaidx
 *
 * Created on 20. března 2016, 22:36
 */

#ifndef USERMESSAGEPARSER_H
#define USERMESSAGEPARSER_H

#include <iostream>
#include <string>
#include <map>

#include "rapidjson/document.h"

#include "ConfigMessage.h"

enum class USER_MESSAGE_TYPE {
    CONFIG, DELETE
};

class UserMessageParser {
public:
    UserMessageParser();
    
    virtual ~UserMessageParser();
    
    void parseMessage(std::string message);
    
    USER_MESSAGE_TYPE getMessageType() const { return m_message_type; };
    
    ConfigMessage getConfigData();
    
private:

    rapidjson::Document m_document;
    
    USER_MESSAGE_TYPE m_message_type;
};


#endif /* USERMESSAGEPARSER_H */
