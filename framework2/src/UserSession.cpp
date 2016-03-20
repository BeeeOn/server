/* 
 * File:   UserSession.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#include "UserSession.h"

#include <iostream>
#include <stdexcept>

#include "rapidjson/document.h"

#include "UserMessageParser.h"

 UserSession::UserSession(asio::io_service& io_service):
    Session(io_service)
{
}

void UserSession::receivedMessage(size_t bytes_transferred)
{
    std::string message = convertMessage(bytes_transferred);
    std::cout << "Msg: " << message;
    
    UserMessageParser user_message_parser;
    try {
        // Parse JSON message.
        user_message_parser.parseMessage(message);
        
        // Find out which type message is.
        switch(user_message_parser.getMessageType()) {
            case(USER_MESSAGE_TYPE::CONFIG):
                // Config message was received.
                ConfigData config_data = user_message_parser.getConfigData();
                
                std::cout << "---CONFIG DATA---" << std::endl;
                std::cout << "USER ID: " << config_data.user_id << std::endl;
                std::cout << "TASK ID: " << config_data.task_id << std::endl;
                std::cout << "INDIVIDUAL ID: " << config_data.individual_id << std::endl;
                std::cout << "PARAMETERS:" << std::endl;
                
                for (auto parameter : config_data.parameters) {
                    std::cout << "P: " << parameter.first << ": " << parameter.second << std::endl; 
                }
                std::cout << std::endl;
                // CREATE OR CHANGE NEW INSTANCE.
                
                break;
        };
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}