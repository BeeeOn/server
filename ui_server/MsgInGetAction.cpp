/* 
 * File:   MsgInGetAction.cpp
 * Author: pavel
 * 
 * Created on 17. září 2014, 14:44
 */

#include "MsgInGetAction.h"
const string MsgInGetAction::state = "getaction";
MsgInGetAction::MsgInGetAction(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}

MsgInGetAction::~MsgInGetAction() {
}

int MsgInGetAction::getMsgAuthorization() {
    return USER;
}

string MsgInGetAction::createResponseMsgOut() {
    string actionId = _doc->child("communication").child("complexaction").attribute("id").value();
    //return new MsgOutAction(_comId, actionId, _state);
    return envelopeResponse(R_TRUE);
}
