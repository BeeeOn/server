/* 
 * File:   MsgInUpdateAction.cpp
 * Author: pavel
 * 
 * Created on 17. září 2014, 15:28
 */

#include "MsgInUpdateAction.h"
#include "DBConnector.h"

const string MsgInUpdateAction::state="updateaction";

MsgInUpdateAction::MsgInUpdateAction(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc){
}


MsgInUpdateAction::~MsgInUpdateAction() {
}

int MsgInUpdateAction::getMsgAuthorization() {
    return USER;
}

string MsgInUpdateAction::createResponseMsgOut() {
    pugi::xml_node node = _doc->child("communication").child("complexaction");
    
    string actionId = node.attribute("id").value();
    string actionName = node.attribute("name").value();    
    
    ostringstream oss;
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
        it->print(oss,"",pugi::format_raw);
    
    string actionXml = oss.str();
    
    //TODO testovat validitu xml a propojit s users
    if(DBConnector::getInstance().updateAction(actionId, actionName, actionXml) ==0)
        throw ServerException(ServerException::WRONG_OR_EMPTY_ACTION);
    
    //return new MsgOutTrue(_comId,_state);
    return envelopeResponse(R_TRUE);
}

