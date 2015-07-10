/* 
 * File:   MsgInGetUserID.cpp
 * Author: pavel
 * 
 * Created on 17. březen 2015, 18:04
 */

#include "UserGetID.h"
#include "../DAO/DAOUsers.h"

using namespace std;

const string UserGetID::state = "getuid1";
UserGetID::UserGetID(pugi::xml_document* doc): IMsgInLoginRequired(doc) {
}


UserGetID::~UserGetID() {
}

int UserGetID::getMsgAuthorization() {
    return GUEST;
}

string UserGetID::createResponseMsgOut() {
    int UID = DAOUsers::getInstance().getUserIdbyIhaToken(_token);
    _outputMainNode.append_attribute(P_USER_ID) = UID;
    return genOutputXMLwithVersionAndState(R_UID);
    
}
