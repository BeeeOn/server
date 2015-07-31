/* 
 * File:   MsgInAddConAccount.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 14:06
 */

#include "AccountAdd.h"
#include "../DAO/DAOUsersAdapters.h"


const std::string MsgInAddAccount::state = "addaccs";

MsgInAddAccount::MsgInAddAccount(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

MsgInAddAccount::~MsgInAddAccount() {
}

int MsgInAddAccount::getMsgAuthorization() {
    return SUPERUSER;
}

string MsgInAddAccount::createResponseMsgOut()
{    
    pugi::xml_node userNode =  _doc->child(P_COMMUNICATION).child(P_USER);
    string newUserMail;
    string newRole;

    newUserMail = userNode.attribute(P_EMAIL).value();
    newRole = userNode.attribute(P_ROLE).value();
    
    if(!isRoleValid(newRole)){
        return getNegativeXMLReply(ServerException::ROLE);
    }else if(DAOUsersAdapters::getInstance().addConAccount(_gatewayId, newUserMail, newRole) != 1){
        return getNegativeXMLReply(ServerException::EMAIL);
    }        
    
    return getXMLreply(R_TRUE);
}
