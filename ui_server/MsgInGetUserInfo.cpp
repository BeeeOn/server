/* 
 * File:   MsgInGetUserInfo.cpp
 * Author: pavel
 * 
 * Created on 21. březen 2015, 18:45
 */

#include "MsgInGetUserInfo.h"
#include "../DAO/DAOUsers.h"

const std::string MsgInGetUserInfo::state = "getuserinfo";

MsgInGetUserInfo::MsgInGetUserInfo(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc){
}


MsgInGetUserInfo::~MsgInGetUserInfo() {
}

int MsgInGetUserInfo::getMsgAuthorization() {
    return GUEST;
}

string MsgInGetUserInfo::createResponseMsgOut() {
    
    User user = DAOUsers::getInstance().getUserAssociatedWithToken(_IHAtoken);
 /*   string attr = string(P_USER_NAME)+"=\""+user.givenName+"\" "+
            P_USER_SURNAME+"=\""+user.familyName+"\" "+
            P_USER_GENDER+"=\""+user.gender+"\" "+
            P_USER_EMAIL+"=\""+user.mail+"\" ";

     return envelopeResponseWithAttributes(R_USER_INFO, attr);*/
    _mainNode.append_attribute(P_USER_NAME) = user.givenName.c_str();
    _mainNode.append_attribute(P_USER_SURNAME) = user.familyName.c_str();
    _mainNode.append_attribute(P_USER_GENDER) = user.gender.c_str();
    _mainNode.append_attribute(P_USER_EMAIL) = user.mail.c_str();

    return genOutputXMLwithVersionAndState(R_USER_INFO);
}




