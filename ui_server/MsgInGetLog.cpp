/* 
 * File:   MsgInLogName.cpp
 * Author: pavel
 * 
 * Created on 23. červen 2014, 11:57
 */

#include "MsgInGetLog.h"
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include "../DAO/DAODevices.h"
using namespace std;

const string MsgInLogName::state = "getlog";

MsgInLogName::MsgInLogName(char* msg, pugi::xml_document* doc):IMsgInLoginAndAdapterAccessRequired(msg, doc) {
}

MsgInLogName::~MsgInLogName() {
}

int MsgInLogName::getMsgAuthorization() {
    return GUEST;
}

string MsgInLogName::createResponseMsgOut()
{                        
    pugi::xml_node comNode =  _doc->child(P_COMMUNICATION);
    string logFrom =  comNode.attribute(P_LOG_FROM).value();
    string logTo = comNode.attribute(P_LOG_TO).value();
    string type =  comNode.attribute(P_LOG_FUNC_TYPE).value();
    string interval = comNode.attribute(P_LOG_INTERVAL).value();
    
    device d;
    d.id =  comNode.attribute(P_DEVICE_ID).value();
    d.type =  comNode.attribute(P_DEVICE_TYPE).value();
    
    return envelopeResponse(R_LOG, DAODevices::getInstance().getXMLDeviceLog(_adapterId, d, logFrom, logTo, type , interval));
}