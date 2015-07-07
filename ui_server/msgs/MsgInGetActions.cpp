/* 
 * File:   MsgInGetActions.cpp
 * Author: pavel
 * 
 * Created on 17. září 2014, 13:41
 */

#include "MsgInGetActions.h"

const string MsgInGetActions::state = "getactions";
MsgInGetActions::MsgInGetActions(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}

MsgInGetActions::~MsgInGetActions() {
}

int MsgInGetActions::getMsgAuthorization() {
    return USER;
}

string MsgInGetActions::createResponseMsgOut() {
    //return new MsgOutActions(_comId, _parredUserId, _state);
    return envelopeResponse(R_TRUE);
}



