/* 
 * File:   MsgInChangeConAccount.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 13:43
 */

#include "AccountUpdate.h"
#include "../DAO/DAOUsers.h"
#include "../DAO/DAOUsersAdapters.h"


const std::string AccountUpdate::state = "setaccs";

AccountUpdate::AccountUpdate(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

AccountUpdate::~AccountUpdate() {
}

int AccountUpdate::getMsgAuthorization() {
    return SUPERUSER;
}

string AccountUpdate::createResponseMsgOut()
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
        
        //check role validity
        if(!isRoleValid(newRole)){
            errText += "<user email=\""+newUserMail+"\" role=\""+newRole+"\"/>";
            fail = ServerException::ROLE;
        }else{
            
            User user = DAOUsers::getInstance().getUserByID(_userId);
            if(newUserMail != user.mail)
            //change users role
             if(DAOUsersAdapters::getInstance().changeConAccount(_gatewayId, newUserMail, newRole) != 1){
                    errText += "<user email=\""+newUserMail+"\" role=\""+newRole+"\"/>";
                    fail = ServerException::EMAIL;
             }
        } 
        
    }
    
    if(fail != 0)
        return getNegativeXMLReply(fail, errText.c_str());
    return getXMLreply(R_TRUE);
}

