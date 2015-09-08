/* 
 * File:   MsgInDelConAccount.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 11:32
 */

#include "AccountDel.h"
#include "../DAO/DAOUsersAdapters.h"
#include "../DAO/DAOUsers.h"


using namespace std;

const string AccountDel::state = "deletegateuser";
AccountDel::AccountDel(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc)  {
}

AccountDel::~AccountDel() {
}

int AccountDel::getMsgAuthorization() {
    return permissions::superuser;
}

string AccountDel::createResponseMsgOut()
{
    int userId;
    
    pugi::xml_node userNode =  _doc->child(proto::communicationNode).child(proto::userNode);
    
    userId = userNode.attribute(proto::userIdAttr).as_int();

    if(userId != _userId)
        if(DAOUsersAdapters::getInstance().delConAccount(_gatewayId, userId) == 0){
            return getNegativeXMLReply(ServerException::USER_ID);
            }
    
    return getXMLreply(proto::replyTrue);
}



