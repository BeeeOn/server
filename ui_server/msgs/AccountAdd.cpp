/* 
 * File:   MsgInAddConAccount.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 14:06
 */

#include "AccountAdd.h"
#include "../DAO/DAOUsersAdapters.h"


const std::string MsgInAddAccount::state = "invitegateuser";

MsgInAddAccount::MsgInAddAccount(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

MsgInAddAccount::~MsgInAddAccount() {
}

int MsgInAddAccount::getMsgAuthorization() {
    return permissions::superuser;
}

string MsgInAddAccount::createResponseMsgOut()
{    
    pugi::xml_node userNode =  _doc->child(proto::communicationNode).child(proto::userNode);
    string newUser;
    string newRole;

    newUser = userNode.attribute(proto::userEmailAttr).value();
    newRole = userNode.attribute(proto::userRoleAttr).value();
    
    if(!isRoleValid(newRole)){
        return getNegativeXMLReply(ServerException::ROLE);
    }else if(DAOUsersAdapters::getInstance().addConAccount(_gatewayId, newUser, newRole) != 1){
        return getNegativeXMLReply(ServerException::USER_ID);
    }        
    
    return getXMLreply(proto::replyTrue);
}
