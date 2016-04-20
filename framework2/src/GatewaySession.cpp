/* 
 * File:   GatewaySession.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 29. March 2016
 */

#include "GatewaySession.h"

#include <vector>

#include "DataMessage.h"
#include "DataMessageParser.h"
#include "DataMessageRegister.h"
#include "Logger.h"
#include "TaskInstance.h"

GatewaySession::GatewaySession(asio::io_service& io_service):
    Session(io_service)
{
}

void GatewaySession::processMessage(std::string message)
{
    try {
        DataMessageParser data_message_parser;
        DataMessage data_message = data_message_parser.parseMessage(message);  
    
        std::string modules;
        for (auto module: data_message.modules) {
            
            modules += "module_id: ";
            modules += module.first;
            modules += " | status: ";
            
            if (module.second.first == MODULE_STATUS::AVAILABLE) {
                modules += "available";
            } 
            else {
                modules += "unavailable";
            }
            modules += "| value: ";
            modules += module.second.second;
            modules += ", ";
        }
        
        logger.LOGFILE("data_message_interface", "MSG") << "Received data message | gateway_id: "
                        << data_message.gateway_id << ", device_euid: " << data_message.device_euid
                        << ", time: " << data_message.time << ", config: { " << modules << "}" << std::endl;

        // Activate all instances which are registered to receive this data.
        DataMessageRegister::getInstance()->activateInstances(data_message);
    }
    catch (const std::exception& e) {
        
        logger.LOGFILE("data_message_interface", "ERROR") << e.what() << std::endl;
    }
}
