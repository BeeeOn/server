/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataMsgSession.cpp
 * Author: mrmaidx
 * 
 * Created on 29. března 2016, 10:47
 */

#include "GatewaySession.h"

#include <iostream>
#include <vector>

#include "DataMessage.h"
#include "DataMessageParser.h"
#include "DataMessageRegister.h"
#include "TaskInstance.h"

GatewaySession::GatewaySession(asio::io_service& io_service):
    Session(io_service)
{
}

void GatewaySession::receivedMessage(size_t bytes_transferred)
{
    std::string message = convertMessage(bytes_transferred);
    std::cout << "Msg: " << message;
    
    try {
        DataMessageParser data_message_parser;
        DataMessage data_message = data_message_parser.parseMessage(message);  
    
        std::cout << "GATEWAY_ID: " << data_message.gateway_id << std::endl;
        std::cout << "DEVICE_EUID: " << data_message.device_euid << std::endl;
        std::cout << "TIME: " << data_message.time << std::endl;

        std::cout << "MODULES: " << std::endl;
        std::string str_status;
        for (auto module : data_message.modules) {
            
            if (module.second.first == MODULE_STATUS::AVAILABLE) {
                str_status = "available";
            } 
            else {
                str_status = "unavailable";
            }
            std::cout << "module_id: " << module.first << " | status: " << str_status << " | value: " << module.second.second << std::endl;

        }
        
        std::vector<TaskInstance*> to_activate = DataMessageRegister::getInstance()->returnAllEntries(data_message.device_euid);
        std::cout << "to activate: " << to_activate.size() << std::endl;
        
        for (auto task_ptr : to_activate) {
            
            task_ptr->activate(data_message);
        }
        
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        //send("Request was not fullfiled. Sorry.");
    }
}
