/* 
 * File:   UserMessageParser.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 23. March 2016
 */

#ifndef USERMESSAGEPARSER_H
#define USERMESSAGEPARSER_H

#include <map>
#include <string>

#include "rapidjson/document.h"

#include "UserMessages.h"

class UserMessageParser {
public:
    /**
     * Parses received message to DOM, and returns it's type.
     * @param message Received message as std::string.
     * @return Type of message.
     */
    USER_MESSAGE_TYPE parseMessage(std::string message);
    /**
     * Process parsed user message as CreateMessage.
     * @return CreateMessage object filled with parsed data.
     */
    CreateMessage processCreateMessage();
    /**
     * Process parsed user message as ChangeMessage.
     * @return ChangeMessage object filled with parsed data.
     */
    ChangeMessage processChangeMessage();
    /**
     * Process parsed user message as DeleteMessage.
     * @return DeleteMessage object filled with parsed data.
     */
    DeleteMessage processDeleteMessage();
    /**
     * Process parsed user message as GetInstIdsMessage.
     * @return GetInstIdsMessage object filled with parsed data.
     */
    GetInstIdsMessage processGetInstIdsMessage();
    /**
     * Process parsed user message as GetConfMessage.
     * @return GetConfMessage object filled with parsed data.
     */
    GetConfMessage processGetConfMessage();
    /**
     * Process parsed user message as GetDataMessage.
     * @return GetDataMessage object filled with parsed data.
     */
    GetDataMessage processGetDataMessage();
    
private:
    /** DOM document to store parsed message. */
    rapidjson::Document m_document;
    /**
     * Returns value of key user_id from m_document.
     */
    long getUserId();
    /**
     * Returns value of key task_id from m_document.
     */
    short getTaskId();
    /**
     * Returns value of key instance_id from m_document.
     */
    long getInstanceId();
    /**
     * Returns map of strings from m_document.
     * @param key_name Name of parameter of string map.
     * @return Map of configuration or parameters.
     */
    std::map<std::string /*name*/, std::string /*value*/> getStringMap(std::string key_name);
};

#endif /* USERMESSAGEPARSER_H */
