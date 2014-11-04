/* 
 * File:   MsgInDelAction.cpp
 * Author: pavel
 * 
 * Created on 17. září 2014, 15:54
 */

#include "MsgInDelAction.h"
#include "DBConnector.h"
const string MsgInDelAction::state= "delaction";
MsgInDelAction::MsgInDelAction(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}


MsgInDelAction::~MsgInDelAction() {
}

int MsgInDelAction::getMsgAuthorization() {
    return USER;
}

string MsgInDelAction::createResponseMsgOut() {
    pugi::xml_node node = _doc->child("communication").child("complexaction");
    
    string actionId = node.attribute("id").value();
    
    DBConnector::getInstance().deleteAction(actionId) ;
    
    //return new MsgOutTrue(_comId, _state);
    return envelopeResponse(R_TRUE);
}


