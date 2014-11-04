/* 
 * File:   MsgInDelCondition.cpp
 * Author: pavel
 * 
 * Created on 16. září 2014, 16:10
 */

#include "MsgInDelCondition.h"
#include "DBConnector.h"

const string MsgInDelCondition::state = "delcondition";
MsgInDelCondition::MsgInDelCondition(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}

MsgInDelCondition::~MsgInDelCondition() {
}

int MsgInDelCondition::getMsgAuthorization() {
    return USER;
}

string MsgInDelCondition::createResponseMsgOut() {
    pugi::xml_node node = _doc->child("communication").child("condition");
    
    string condId = node.attribute("id").value();
    
    DBConnector::getInstance().deleteCondition(condId) ;
    
    //return new MsgOutTrue(_comId, _state);
    return envelopeResponse(R_TRUE);
}


