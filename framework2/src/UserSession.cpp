/* 
 * File:   UserSession.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#include "UserSession.h"

#include <stdexcept>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "DatabaseInterface.h"
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
                CreateMessage create_message = user_message_parser.processCreateMessage();

                std::string log_config;
                for (auto member: create_message.config) {
                    log_config += "name: ";
                    log_config += member.first;
                    log_config += " | value: ";
                    log_config += member.second;
                    log_config += ", ";
                }
                logger.LOGFILE("user_message_interface", "MSG") << "Received message: CREATE | user_id: "
                        << create_message.user_id << ", task_id: " << create_message.task_id
                        << ", config: { " << log_config << "}" << std::endl;

                // Find task and get pointer to it's object.
                std::shared_ptr<Task> task = TaskLoader::getInstance()->findTask(create_message.task_id);
                // Create instance in database.
                long instance_id = task->getTaskManagerPtr()->createInstance(create_message);
                // Create instance in BAF and store configuration.
                task->getTaskManagerPtr()->createConfiguration(instance_id, create_message.config);

                // Construct message to send back to client.
                response.AddMember("error", false, response.GetAllocator());
                response.AddMember("instance_id", instance_id, response.GetAllocator());
            }
            break;
            case(USER_MESSAGE_TYPE::CHANGE):
            {
                ChangeMessage change_message = user_message_parser.processChangeMessage();
                
                std::string log_config;
                for (auto member: change_message.config) {
                    log_config += "name: ";
                    log_config += member.first;
                    log_config += " | value: ";
                    log_config += member.second;
                    log_config += ", ";
                }
                logger.LOGFILE("user_message_interface", "MSG") << "Received message: CHANGE | user_id: "
                        << change_message.user_id << ", task_id: " << change_message.task_id
                        << ", instance_id: " << change_message.instance_id << ", config: { "
                        << log_config << "}" << std::endl;

                // Check if user with user_id owns instance with instance_id.
                checkIfUserOwnsInstance(change_message.user_id, change_message.instance_id);
                
                // Find task and get pointer to it's object.
                std::shared_ptr<Task> task = TaskLoader::getInstance()->findTask(change_message.task_id);
                // Change instances configuration in database.
                task->getTaskManagerPtr()->changeConfiguration(change_message);

                // Construct message to send back to client.
                response.AddMember("error", false, response.GetAllocator());
            }
            break;
            case(USER_MESSAGE_TYPE::DELETE):
            {
                DeleteMessage delete_message = user_message_parser.processDeleteMessage();

                logger.LOGFILE("user_message_interface", "MSG") << "Received message: DELETE | user_id: "
                        << delete_message.user_id << ", task_id: " << delete_message.task_id
                        << ", instance_id: " << delete_message.instance_id << std::endl;

                // Check if user with user_id owns instance with instance_id.
                checkIfUserOwnsInstance(delete_message.user_id, delete_message.instance_id);
                
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
                GivePermMessage give_perm_message = user_message_parser.processGivePermMessage();

                logger.LOGFILE("user_message_interface", "MSG") << "Received message: GIVE_PERM | user_id: "
                        << give_perm_message.user_id << ", task_id: " << give_perm_message.task_id
                        << ", instance_id: " << give_perm_message.instance_id << ", friend_mail: "
                        << give_perm_message.friend_mail << ", permission: "
                        << give_perm_message.permission << std::endl;

                // Check if user with user_id owns instance with instance_id.
                checkIfUserOwnsInstance(give_perm_message.user_id, give_perm_message.instance_id);
                
                // Construct message to send back to client.
                // Functionality is not yet implemented.
                response.AddMember("error", true, response.GetAllocator());
            }
            break;
            case(USER_MESSAGE_TYPE::GET_INST_IDS):
            {
                GetInstIdsMessage get_inst_ids_message = user_message_parser.processGetInstIdsMessage();

                logger.LOGFILE("user_message_interface", "MSG") << "Received message: GET_INST | user_id: "
                        << get_inst_ids_message.user_id << ", task_id: " << get_inst_ids_message.task_id
                        << std::endl;

                // Find task and get pointer to it's object.
                std::shared_ptr<Task> task = TaskLoader::getInstance()->findTask(get_inst_ids_message.task_id);
                // Get ids of all instances owned by user of received task_id.
                std::vector<long> instance_ids = task->getTaskManagerPtr()->getInstanceIds(get_inst_ids_message);

                // Create array of instances to add to response.
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
            case(USER_MESSAGE_TYPE::GET_CONF):
            {
                GetConfMessage get_conf_message = user_message_parser.processGetConfMessage();

                logger.LOGFILE("user_message_interface", "MSG") << "Received message: GET_CONF | user_id: "
                        << get_conf_message.user_id << ", task_id: " << get_conf_message.task_id
                        << ", instance_id: " << get_conf_message.instance_id << std::endl;

                // Check if user with user_id owns instance with instance_id.
                checkIfUserOwnsInstance(get_conf_message.user_id, get_conf_message.instance_id);
                
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
            case(USER_MESSAGE_TYPE::GET_DATA):
            {
                GetDataMessage get_data_message = user_message_parser.processGetDataMessage();

                std::string log_parameters;
                for (auto member: get_data_message.parameters) {
                    log_parameters += "name: ";
                    log_parameters += member.first;
                    log_parameters += " | value: ";
                    log_parameters += member.second;
                    log_parameters += ", ";
                }
                logger.LOGFILE("user_message_interface", "MSG") << "Received message: CHANGE | user_id: "
                        << get_data_message.user_id << ", task_id: " << get_data_message.task_id
                        << ", instance_id: " << get_data_message.instance_id << ", config: { "
                        << log_parameters << "}" << std::endl;
                
                // Check if user with user_id owns instance with instance_id.
                checkIfUserOwnsInstance(get_data_message.user_id, get_data_message.instance_id);
                
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
        logger.LOGFILE("user_message_interface", "ERROR") << e.what() << std::endl;

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

void UserSession::checkIfUserOwnsInstance(long user_id, long instance_id)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    short owns;
    *sql << "SELECT exists(SELECT 1 FROM user_instance WHERE user_id = :user_id AND instance_id = :instance_id);",
            soci::use(user_id, "user_id"),
            soci::use(instance_id, "instance_id"),
            soci::into(owns);
                
    if (!owns) {
        std::stringstream error;
        error << "User with user_id: " << user_id << " tried to change instance_id: "
              << instance_id << " which he doesn't own.";
        throw std::runtime_error(error.str());
    }
}
