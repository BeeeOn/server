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
    
    string errText;
    int fail = 0;
    
    for (; userNode; userNode = userNode.next_sibling(P_USER))
    {
        newUserMail = userNode.attribute(P_EMAIL).value();
        newRole = userNode.attribute(P_ROLE).value();
        
        
        if(!isRoleValid(newRole)){
            errText += "<" P_USER " " P_EMAIL "=\""+newUserMail+"\" " P_ROLE "=\""+newRole+"\" />";
            fail = ServerException::ROLE;
        }else if(DAOUsersAdapters::getInstance().addConAccount(_adapterId, newUserMail, newRole) != 1){
            errText += "<" P_USER " " P_EMAIL "=\""+newUserMail+"\" " P_ROLE "=\""+newRole+"\" />";
            fail = ServerException::EMAIL;
        } 
    }
    if(fail != 0)
        return getNegativeXMLReply(fail, errText.c_str());
    
    return getXMLreply(R_TRUE);
}
