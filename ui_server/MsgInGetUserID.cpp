/* 
 * File:   MsgInGetUserID.cpp
 * Author: pavel
 * 
 * Created on 17. březen 2015, 18:04
 */

#include "MsgInGetUserID.h"
#include "../DAO/DAOUsers.h"

using namespace std;

const string MsgInGetUserID::state = "getuid1";
MsgInGetUserID::MsgInGetUserID(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}


MsgInGetUserID::~MsgInGetUserID() {
}

int MsgInGetUserID::getMsgAuthorization() {
    return GUEST;
}

string MsgInGetUserID::createResponseMsgOut() {
    
    string UID = DAOUsers::getInstance().getUserID(_token);
    return envelopeResponse(R_UID, "uid=\""+UID+"\" ");
}
