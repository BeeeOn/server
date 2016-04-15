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

#include "UserMessages.h"
#include "Task.h"
#include "TaskLoader.h"
#include "UserMessageParser.h"

UserSession::UserSession(asio::io_service& io_service):
    Session(io_service)
{
}

void UserSession::processMessage(std::string message)
{
    
    //std::cout << "Msg: " << message;
    
    UserMessageParser user_message_parser;
    try {
        // Parse JSON message.
        USER_MESSAGE_TYPE type = user_message_parser.parseMessage(message);
        
        // Find out which type message is.
        switch(type) {
            case(USER_MESSAGE_TYPE::CREATE):
            {
                std::cout << "USER_MESSAGE_TYPE::CREATE" << std::endl;
                
                CreateMessage create_message = user_message_parser.processCreateMessage();
                
                std::cout << "USER_ID: " << create_message.user_id << " | TASK_ID: " << create_message.task_id << std::endl;
                
                for (auto entry : create_message.config ) {
                    std::cout << " | KEY: " << entry.first << " | VAL: " << entry.second;
                }
                std::cout << std::endl;
                
                // Find task and get pointer to it's object.
                std::shared_ptr<Task> task = TaskLoader::getInstance()->findTask(create_message.task_id);
                // Create instance in database.
                long instance_id = task->getTaskManagerPtr()->createInstance(create_message);
                // Create instance in program and store configuration.
                task->getTaskManagerPtr()->createConfiguration(instance_id, create_message.config);
                // Send back instance_id.
                sendResponse(std::string("instance_id: ") + std::to_string(instance_id) + std::string("\n"));
            }
            break;
            case(USER_MESSAGE_TYPE::CHANGE):
            {
                std::cout << "USER_MESSAGE_TYPE::CHANGE" << std::endl;
                
                ChangeMessage change_message = user_message_parser.processChangeMessage();
                
                std::cout << "USER_ID: " << change_message.user_id << " | TASK_ID: " << change_message.task_id;
                std::cout << " | INST_ID: " << change_message.instance_id << std::endl;
                
                for (auto entry : change_message.config ) {
                    std::cout << " | KEY: " << entry.first << " | VAL: " << entry.second;
                }                
                std::cout << std::endl;
                
                // Find task and get pointer to it's object.
                std::shared_ptr<Task> task = TaskLoader::getInstance()->findTask(change_message.task_id);
                // Change instance.
                task->getTaskManagerPtr()->changeConfiguration(change_message);
            }
            break;
            case(USER_MESSAGE_TYPE::DELETE):
            {
                std::cout << "USER_MESSAGE_TYPE::DELETE" << std::endl;
                
                DeleteMessage delete_message = user_message_parser.processDeleteMessage();
                
                std::cout << "USER_ID: " << delete_message.user_id << " | TASK_ID: " << delete_message.task_id;
                std::cout << " | INST_ID: " << delete_message.instance_id << std::endl;
                
                // Find task and get pointer to it's object.
                std::shared_ptr<Task> task = TaskLoader::getInstance()->findTask(delete_message.task_id);
                // Delete instance.
                task->getTaskManagerPtr()->deleteInstance(delete_message);
               
            }
            break;
            case(USER_MESSAGE_TYPE::GIVE_PERM):
            {
                std::cout << "USER_MESSAGE_TYPE::GIVE_PERM" << std::endl;
                
                GivePermMessage give_perm_message = user_message_parser.processGivePermMessage();
                
                std::cout << "USER_ID: " << give_perm_message.user_id << " | TASK_ID: " << give_perm_message.task_id;
                std::cout << "INST_ID: " << give_perm_message.instance_id << " | FRIEND_M: " << give_perm_message.friend_mail;
                std::cout << " | PERMISS: " << give_perm_message.permission << std::endl;
                
            }
            break;
            case(USER_MESSAGE_TYPE::GET_INST_IDS): {
                std::cout << "USER_MESSAGE_TYPE::GET_INST_IDS" << std::endl;
                
                GetInstIdsMessage get_inst_ids_message = user_message_parser.processGetInstIdsMessage();
                
                std::cout << "USER_ID: " << get_inst_ids_message.user_id << " | TASK_ID: " << get_inst_ids_message.task_id << std::endl;
            
                // Find task and get pointer to it's object.
                std::shared_ptr<Task> task = TaskLoader::getInstance()->findTask(get_inst_ids_message.task_id);
                std::vector<std::string> instance_ids = task->getTaskManagerPtr()->getInstanceIds(get_inst_ids_message);
                
                std::string response;
                for (auto instance_id : instance_ids) {
                    response += instance_id;
                    response += " | ";
                }
                response += "\n";
                sendResponse(response);
                
            }
            break;
            case(USER_MESSAGE_TYPE::GET_CONF): {
                std::cout << "USER_MESSAGE_TYPE::GET_CONF" << std::endl;
                
                GetConfMessage get_conf_message = user_message_parser.processGetConfMessage();
                
                std::cout << "USER_ID: " << get_conf_message.user_id << " | TASK_ID: " << get_conf_message.task_id;
                std::cout << " | INST_ID: " << get_conf_message.instance_id << std::endl;
                
                // Find task and get pointer to it's object.
                std::shared_ptr<Task> task = TaskLoader::getInstance()->findTask(get_conf_message.task_id);
                // Get map of configuration.
                std::map<std::string, std::string> map = task->getTaskManagerPtr()->getConfiguration(get_conf_message);
            
                std::string response;
                for (auto key : map ) {
                    response += key.first;
                    response += " | ";
                    response += key.second;
                    response += "\n";
                }
                sendResponse(response);
            }
            break;
            case(USER_MESSAGE_TYPE::GET_DATA): {
                std::cout << "USER_MESSAGE_TYPE::GET_DATA" << std::endl;
                
                GetDataMessage get_data_message = user_message_parser.processGetDataMessage();
                
                std::cout << "USER_ID: " << get_data_message.user_id << " | TASK_ID: " << get_data_message.task_id;
                std::cout << " | INST_ID: " << get_data_message.instance_id << std::endl;
                
                for (auto entry : get_data_message.parameters ) {
                    std::cout << " | KEY: " << entry.first << " | VAL: " << entry.second;
                }   
                std::cout << std::endl;
            }
            break;
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        sendResponse("Request was not fullfiled. Sorry.");
    }
}

