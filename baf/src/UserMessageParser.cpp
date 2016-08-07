/*
 * File:   UserMessageParser.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 23. March 2016
 */

#include "UserMessageParser.h"

#include <map>
#include <sstream>
#include <string>

#include "rapidjson/document.h"

#include "Logger.h"

USER_MESSAGE_TYPE UserMessageParser::parseMessage(std::string message)
{
	// Parse JSON message to DOM document.
	rapidjson::ParseResult result = m_document.Parse(message.c_str());
	if (!result) {
		logger.LOGFILE("user_message_interface", "ERROR") << "UserMessageParser: JSON parsing error." << std::endl;
		throw std::runtime_error("UserMessageParser: Processing of received message was not successful.");
	}

	if (!m_document.IsObject()) {
		throw std::runtime_error("UserMessageParser: Received JSON message is not valid object.");
	}
	// Find "msg" value.
	if (m_document.HasMember("msg")) {
		rapidjson::Value& msg_value = m_document["msg"];

		if (!msg_value.IsString()) {
			throw std::runtime_error("UserMessageParser: Received JSON message has member \"msg\", but it's not a string.");
		}
		else {
			std::string type_as_string = msg_value.GetString();

			// Return type of received message.
			if (type_as_string == "create") {
				return USER_MESSAGE_TYPE::CREATE;
			}
			else if (type_as_string == "change") {
				return USER_MESSAGE_TYPE::CHANGE;
			}
			else if (type_as_string == "delete") {
				return USER_MESSAGE_TYPE::DELETE;
			}
			else if (type_as_string == "get_inst_ids") {
				return USER_MESSAGE_TYPE::GET_INST_IDS;
			}
			else if (type_as_string == "get_conf") {
				return USER_MESSAGE_TYPE::GET_CONF;
			}
			else if (type_as_string == "get_data") {
				return USER_MESSAGE_TYPE::GET_DATA;
			}
			else {
				throw std::runtime_error("UserMessageParser: Type of received JSON message was not recognised: " + type_as_string);
			}
		}
	}
	else {
		throw std::runtime_error("UserMessageParser: Received JSON message doesn't have member \"msg\". Type of message is not clear.");
	}
}

long UserMessageParser::getUserId()
{
	if (m_document.HasMember("user_id")) {
		rapidjson::Value& user_id_value = m_document["user_id"];

		if (!user_id_value.IsInt()) {
			throw std::runtime_error("UserMessageParser: Received message has member \"user_id\", but it's not an integer.");
		}
		else {
			// Return user_id.
			return user_id_value.GetInt();
		}
	}
	else {
		throw std::runtime_error("UserMessageParser: Received message doesn't have member \"user_id\".");
	}
}

short UserMessageParser::getTaskId()
{
	if (m_document.HasMember("task_id")) {
		rapidjson::Value& task_id_value = m_document["task_id"];

		if (!task_id_value.IsInt()) {
			throw std::runtime_error("UserMessageParser: Received message has member \"task_id\", but it's not an integer.");
		}
		else {
			// Return task_id.
			return task_id_value.GetInt();
		}
	}
	else {
		throw std::runtime_error("UserMessageParser: Received message doesn't have member \"task_id\".");
	}
}

long UserMessageParser::getInstanceId()
{
	if (m_document.HasMember("instance_id")) {
		rapidjson::Value& instance_id_value = m_document["instance_id"];

		if (!instance_id_value.IsInt()) {
			throw std::runtime_error("UserMessageParser: Received message has member \"instance_id\", but it's not an integer.");
		}
		else {
			// Return task_id.
			return instance_id_value.GetInt();
		}
	}
	else {
		throw std::runtime_error("UserMessageParser: Received message doesn't have member \"instance_id\".");
	}
}

std::map<std::string, std::string> UserMessageParser::getStringMap(std::string key_name)
{
	std::map<std::string, std::string> map;

	if (m_document.HasMember(key_name.c_str())) {
		rapidjson::Value& map_value = m_document[key_name.c_str()];

		if (!map_value.IsObject()) {
			std::stringstream error;
			error << "UserMessageParser: Received message has member \"" << key_name << "\", but it's not an object." << std::endl;
			throw std::runtime_error(error.str());
		}
		else {
			for (rapidjson::Value::ConstMemberIterator itr = map_value.MemberBegin(); itr != map_value.MemberEnd(); itr++) {

				std::string name = itr->name.GetString();

				if (!(itr->value.IsString())) {
					std::stringstream error;
					error << "UserMessageParser: Received message has entry: " << name << " but it is not string type." ;
					throw std::runtime_error(error.str());
				}
				else {
					map.emplace(name, std::string(itr->value.GetString()));
				}
			}
		}
		return map;
	}
	else {
		throw std::runtime_error("UserMessageParser: Received message doesn't have member object \"parameters\".");
	}
}

CreateMessage UserMessageParser::processCreateMessage()
{
	CreateMessage message;
	message.user_id = getUserId();
	message.task_id = getTaskId();
	message.config = getStringMap("config");
	return message;
}

ChangeMessage UserMessageParser::processChangeMessage()
{
	ChangeMessage message;
	message.user_id = getUserId();
	message.task_id = getTaskId();
	message.instance_id = getInstanceId();
	message.config = getStringMap("config");
	return message;
}

DeleteMessage UserMessageParser::processDeleteMessage()
{
	DeleteMessage message;
	message.user_id = getUserId();
	message.task_id = getTaskId();
	message.instance_id = getInstanceId();
	return message;
}

GetInstIdsMessage UserMessageParser::processGetInstIdsMessage()
{
	GetInstIdsMessage message;
	message.user_id = getUserId();
	message.task_id = getTaskId();
	return message;
}

GetConfMessage UserMessageParser::processGetConfMessage()
{
	GetConfMessage message;
	message.user_id = getUserId();
	message.task_id = getTaskId();
	message.instance_id = getInstanceId();
	return message;
}

GetDataMessage UserMessageParser::processGetDataMessage()
{
	GetDataMessage message;
	message.user_id = getUserId();
	message.task_id = getTaskId();
	message.instance_id = getInstanceId();
	message.parameters = getStringMap("parameters");
	return message;
}
