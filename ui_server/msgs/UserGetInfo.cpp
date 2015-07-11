/* 
 * File:   MsgInGetUserInfo.cpp
 * Author: pavel
 * 
 * Created on 21. březen 2015, 18:45
 */

#include "UserGetInfo.h"
#include "../DAO/DAOUsers.h"
#include "pugixml.hpp"

using namespace std;

const std::string UserGetInfo::state = "getuserinfo";

UserGetInfo::UserGetInfo(pugi::xml_document* doc): IMsgInLoginRequired(doc){
}


UserGetInfo::~UserGetInfo() {
}

int UserGetInfo::getMsgAuthorization() {
    return GUEST;
}

string UserGetInfo::createResponseMsgOut() {
    
    User user = DAOUsers::getInstance().getUserByID(_userId);
 /*   string attr = string(P_USER_NAME)+"=\""+user.givenName+"\" "+
            P_USER_SURNAME+"=\""+user.familyName+"\" "+
            P_USER_GENDER+"=\""+user.gender+"\" "+
            P_USER_EMAIL+"=\""+user.mail+"\" ";


     return envelopeResponseWithAttributes(R_USER_INFO, attr);*/
    _outputMainNode.append_attribute(P_USER_ID) = user.user_id;
    _outputMainNode.append_attribute(P_USER_NAME) = user.givenName.c_str();
    _outputMainNode.append_attribute(P_USER_SURNAME) = user.familyName.c_str();
    _outputMainNode.append_attribute(P_USER_GENDER) = user.gender.c_str();
    _outputMainNode.append_attribute(P_USER_EMAIL) = user.mail.c_str();
    _outputMainNode.append_attribute(P_USER_PICTURE) = user.picture.c_str();

    pugi::xml_node accountsNode = _outputMainNode.append_child();
    accountsNode.set_name(P_USER_ACCOUNTS);
    
    pugi::xml_node serviceNode;
    
    serviceNode = accountsNode.append_child();
    serviceNode.set_name(P_SERVICE);
    serviceNode.append_attribute(P_SERVICE_NAME) = P_SERVICE_GOOGLE;
    serviceNode.append_attribute(P_SERVICE_ID) = user.googleId.c_str();
    
    serviceNode = accountsNode.append_child();
    serviceNode.set_name(P_SERVICE);
    serviceNode.append_attribute(P_SERVICE_NAME) = P_SERVICE_FACEBOOK;
    serviceNode.append_attribute(P_SERVICE_ID) = user.facebookId.c_str();
    
    //accountsNode.append_attribute(P_SERVICE_BEEEON)
    
    return getXMLreply(R_USER_INFO);
}




