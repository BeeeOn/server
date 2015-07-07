/* 
 * File:   msgInGetCondition.cpp
 * Author: pavel
 * 
 * Created on 16. září 2014, 11:39
 */

#include "msgInGetCondition.h"

const string msgInGetCondition::state = "getcondition";

msgInGetCondition::msgInGetCondition(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}

msgInGetCondition::~msgInGetCondition() {
}

int msgInGetCondition::getMsgAuthorization() {
    return USER;
}

string msgInGetCondition::createResponseMsgOut() {
    string condId = _doc->child("communication").child("condition").attribute("id").value();
    //return new MsgOutCondition(_comId, condId, _state);
    return envelopeResponse(R_TRUE);
}

