/* 
 * File:   MsgInChangeConAccount.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 13:43
 */

#include "AccountUpdate.h"
#include "../DAO/DAOUsers.h"
#include "../DAO/DAOUsersAdapters.h"


const std::string AccountUpdate::state = "updategateuser";

AccountUpdate::AccountUpdate(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

AccountUpdate::~AccountUpdate() {
}

int AccountUpdate::getMsgAuthorization() {
    return permissions::superuser;
}

string AccountUpdate::createResponseMsgOut()
{
    
    int newUserId;
    string newRole;
    string errText;
    int fail = 0;
    
    pugi::xml_node userNode =  _doc->child(proto::communicationNode).child(proto::userNode);
    newUserId = userNode.attribute(proto::userIdAttr).as_int(-1);
    newRole = userNode.attribute(proto::userRoleAttr).value();

    //check role validity
    if(!isRoleValid(newRole)){
        fail = ServerException::ROLE;
    }else{
        if(newUserId != _userId)
        //change users role
         if(DAOUsersAdapters::getInstance().changeConAccount(_gatewayId, newUserId, newRole) != 1){
                fail = ServerException::USER_ID;
         }
    } 
    
    if(fail != 0)
        return getNegativeXMLReply(fail, errText.c_str());
    return getXMLreply(proto::replyTrue);
}

