/* 
 * File:   MsgInLogName.cpp
 * Author: pavel
 * 
 * Created on 23. červen 2014, 11:57
 */

#include "LogGet.h"
#include "../lib/pugixml.hpp"
#include "../DAO/DAODevices.h" 
#include "../DAO/DAOlogs.h"
using namespace std;

const string GetLog::state = "getlogs";

GetLog::GetLog(pugi::xml_document* doc):IMsgInLoginAndAdapterAccessRequired(doc) {
}

GetLog::~GetLog() {
}

int GetLog::getMsgAuthorization() {
    return permissions::guest;
}

string GetLog::createResponseMsgOut()
{                        
    pugi::xml_node logsNode =  _doc->child(proto::communicationNode).child(proto::logsNode);

    string logFrom =  logsNode.attribute(proto::logsFromAttr).value();
    string logTo = logsNode.attribute(proto::logsToAttr).value();
    string type =  logsNode.attribute(proto::logsFunctionAttr).value();
    string interval = logsNode.attribute(proto::logsIntervalAttr).value();
    
    device d;
    d.id =  logsNode.attribute(proto::logsDeviceIdAttr).value();
    d.type =  logsNode.attribute(proto::logsModuleIdAttr).value();
    
    return envelopeResponse(proto::replyLogs, DAOlogs::getInstance().getXMLDeviceLog(_gatewayId, d, logFrom, logTo, type , interval));
}