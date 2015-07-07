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
MsgInGetUserID::MsgInGetUserID(pugi::xml_document* doc): IMsgInLoginRequired(doc) {
}


MsgInGetUserID::~MsgInGetUserID() {
}

int MsgInGetUserID::getMsgAuthorization() {
    return GUEST;
}

string MsgInGetUserID::createResponseMsgOut() {
    int UID = DAOUsers::getInstance().getUserIdbyIhaToken(_token);
    _outputMainNode.append_attribute(P_USER_ID) = UID;
    return genOutputXMLwithVersionAndState(R_UID);
    
}
