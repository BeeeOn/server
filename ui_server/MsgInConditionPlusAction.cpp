/* 
 * File:   MsgInConditionPlusAction.cpp
 * Author: pavel
 * 
 * Created on 17. září 2014, 16:08
 */

#include "MsgInConditionPlusAction.h"
#include "DBConnector.h"

const string MsgInConditionPlusAction::state = "conditionplusaction";
MsgInConditionPlusAction::MsgInConditionPlusAction(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc)  {
}

MsgInConditionPlusAction::~MsgInConditionPlusAction() {
}

int MsgInConditionPlusAction::getMsgAuthorization() {
    return USER;
}

string MsgInConditionPlusAction::createResponseMsgOut() {

    string condId = _doc->child("communication").child("condition").attribute("id").value();
    string actionId = _doc->child("communication").child("action").attribute("id").value();
    
   if( DBConnector::getInstance().connectConditionWithAction(condId, actionId) ==0) 
       throw ServerException(ServerException::CONDITION_ACTION);
    
    //return new MsgOutTrue(_comId, _state);
    return envelopeResponse(R_TRUE);
}


