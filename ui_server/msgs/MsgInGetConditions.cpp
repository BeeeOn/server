/* 
 * File:   MsgInGetConditions.cpp
 * Author: pavel
 * 
 * Created on 16. září 2014, 15:23
 */

#include "MsgInGetConditions.h"

const string MsgInGetConditions::state = "getconditions";
MsgInGetConditions::MsgInGetConditions(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}
MsgInGetConditions::~MsgInGetConditions() {
}

int MsgInGetConditions::getMsgAuthorization() {
    return USER;
}

string MsgInGetConditions::createResponseMsgOut() {
    //return new MsgOutConditions(_comId,_parredUserMail,_state);
    return envelopeResponse(R_TRUE);
}



