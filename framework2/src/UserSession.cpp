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
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "Logger.h"
#include "Task.h"
#include "TaskLoader.h"
#include "UserMessages.h"
#include "UserMessageParser.h"

UserSession::UserSession(asio::io_service& io_service):
    Session(io_service)
{
}

void UserSession::processMessage(std::string message)
{
    // Type of received message.
    USER_MESSAGE_TYPE type;
    
    // Document in which is constructed JSON response.
    rapidjson::Document response;
    response.SetObject();
    
    UserMessageParser user_message_parser;
    try {
        // Parse JSON message.
        type = user_message_parser.parseMessage(message);
        
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
                
                // Construct message to send back to client.
                response.AddMember("error", false, response.GetAllocator());
                response.AddMember("instance_id", instance_id, response.GetAllocator());
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
                
                // Construct message to send back to client.
                response.AddMember("error", false, response.GetAllocator());
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
                
                // Construct message to send back to client.
                response.AddMember("error", false, response.GetAllocator());
            }
            break;
            case(USER_MESSAGE_TYPE::GIVE_PERM):
            {
                std::cout << "USER_MESSAGE_TYPE::GIVE_PERM" << std::endl;
                
                GivePermMessage give_perm_message = user_message_parser.processGivePermMessage();
                
                std::cout << "USER_ID: " << give_perm_message.user_id << " | TASK_ID: " << give_perm_message.task_id;
                std::cout << "INST_ID: " << give_perm_message.instance_id << " | FRIEND_M: " << give_perm_message.friend_mail;
                std::cout << " | PERMISS: " << give_perm_message.permission << std::endl;
                
                // Construct message to send back to client.
                response.AddMember("error", true, response.GetAllocator());
            }
            break;
            case(USER_MESSAGE_TYPE::GET_INST_IDS): {
                std::cout << "USER_MESSAGE_TYPE::GET_INST_IDS" << std::endl;
                
                GetInstIdsMessage get_inst_ids_message = user_message_parser.processGetInstIdsMessage();
                
                std::cout << "USER_ID: " << get_inst_ids_message.user_id << " | TASK_ID: " << get_inst_ids_message.task_id << std::endl;
            
                // Find task and get pointer to it's object.
                std::shared_ptr<Task> task = TaskLoader::getInstance()->findTask(get_inst_ids_message.task_id);
                std::vector<long> instance_ids = task->getTaskManagerPtr()->getInstanceIds(get_inst_ids_message);
                
                // Create array of instances.
                rapidjson::Value instance_array(rapidjson::kArrayType);

                // Add all instace_ids to array.
                for (auto instance_id : instance_ids) {

                    instance_array.PushBack(instance_id, response.GetAllocator());
                }
                
                // Add member error to response. Set to false.
                response.AddMember("error", false, response.GetAllocator());
                
                // Add array to response.
                response.AddMember("config", instance_array, response.GetAllocator());
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
                std::map<std::string, std::string> config_map = task->getTaskManagerPtr()->getConfiguration(get_conf_message);
            
                // Create config JSON object.
                rapidjson::Value config(rapidjson::kObjectType);
                config.SetObject();
                
                // Store every config item to JSON config object.
                for (auto conf_item : config_map) {    
                    // First create name string.
                    rapidjson::Value name;
                    name.SetString(conf_item.first.c_str(), response.GetAllocator());
                    // Second create value string.
                    rapidjson::Value value;
                    value.SetString(conf_item.second.c_str(), response.GetAllocator());
                    // Add them to config object.
                    config.AddMember(name, value, response.GetAllocator());
                }
                
                // Add member error to response. Set to false.
                response.AddMember("error", false, response.GetAllocator());
                
                // Add config object to JSON response.
                response.AddMember("config", config, response.GetAllocator());
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
                
                // Find task and get pointer to it's object.
                std::shared_ptr<Task> task = TaskLoader::getInstance()->findTask(get_data_message.task_id);
                // Get data from manager.
                std::string data = task->getTaskManagerPtr()->getData(get_data_message);
                // Send already constructed JSON to client.
                sendResponse(data);
            }
            break;
        }
    }
    catch (const std::exception& e) {
        logger.LOGFILE("user_interface", "ERROR") << e.what() << std::endl;

        response.AddMember("error", true, response.GetAllocator());
        // If type of message is GET_DATA, it must be send here.
        if (type == USER_MESSAGE_TYPE::GET_DATA) {
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            response.Accept(writer);
            sendResponse(buffer.GetString());
        }
        
    }
    
    // GET_DATA message sends response on its own.
    if (type != USER_MESSAGE_TYPE::GET_DATA) {
        // Convert response document to std::string.
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        response.Accept(writer);

        // Send response.
        sendResponse(buffer.GetString());
    }
}
