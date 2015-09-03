/* 
 * File:   MsgInGetUserInfo.cpp
 * Author: pavel
 * 
 * Created on 21. březen 2015, 18:45
 */

#include "UserGetInfo.h"
#include "../DAO/DAOUsers.h"
#include "../lib/pugixml.hpp"

using namespace std;

const std::string UserGetInfo::state = "getuserinfo";

UserGetInfo::UserGetInfo(pugi::xml_document* doc): IMsgInLoginRequired(doc){
}


UserGetInfo::~UserGetInfo() {
}

int UserGetInfo::getMsgAuthorization() {
    return permissions::guest;
}

string UserGetInfo::createResponseMsgOut() {
    
    User user = DAOUsers::getInstance().getUserByID(_userId);
 /*   string attr = string(proto::userNameAttr)+"=\""+user.givenName+"\" "+
            proto::userSurnameAttr+"=\""+user.familyName+"\" "+
            proto::userGenderAttr+"=\""+user.gender+"\" "+
            proto::userEmailAttr+"=\""+user.mail+"\" ";


     return envelopeResponseWithAttributes(proto::replyUserInfo, attr);*/
    pugi::xml_node userNode = _outputMainNode.append_child();
    userNode.set_name(proto::userNode);
    userNode.append_attribute(proto::userIdAttr) = user.user_id;
    userNode.append_attribute(proto::userNameAttr) = user.givenName.c_str();
    userNode.append_attribute(proto::userSurnameAttr) = user.familyName.c_str();
    userNode.append_attribute(proto::userGenderAttr) = user.gender.c_str();
    userNode.append_attribute(proto::userEmailAttr) = user.mail.c_str();
    userNode.append_attribute(proto::userPictureAttr) = user.picture.c_str();

    pugi::xml_node accountsNode = _outputMainNode.append_child();
    accountsNode.set_name(proto::userAccountsAttr);
    
    pugi::xml_node serviceNode;
    
    if(user.googleId != ""){
        serviceNode = accountsNode.append_child();
        serviceNode.set_name(proto::authProviderNode);
        serviceNode.append_attribute(proto::authProvideNameAttr) = proto::authServiceGoogleAttr;
        serviceNode.append_attribute(proto::authProviderIdAttr) = user.googleId.c_str();
    }
    
    if(user.facebookId != ""){
        serviceNode = accountsNode.append_child();
        serviceNode.set_name(proto::authProviderNode);
        serviceNode.append_attribute(proto::authProvideNameAttr) = proto::authServiceFacebookAttr;
        serviceNode.append_attribute(proto::authProviderIdAttr) = user.facebookId.c_str();
    }
    //accountsNode.append_attribute(proto::authServiceBeeeonAttr)
    
    return getXMLreply(proto::replyUserInfo);
}




