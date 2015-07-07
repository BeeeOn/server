/* 
 * File:   MsgInChangeConAccount.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 13:43
 */

#include "MsgInChangeAccount.h"
#include "../DAO/DAOUsers.h"
#include "../DAO/DAOUsersAdapters.h"


const std::string MsgInChangeConAccount::state = "setaccs";

MsgInChangeConAccount::MsgInChangeConAccount(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

MsgInChangeConAccount::~MsgInChangeConAccount() {
}

int MsgInChangeConAccount::getMsgAuthorization() {
    return SUPERUSER;
}

string MsgInChangeConAccount::createResponseMsgOut()
{
    pugi::xml_node userNode =  _doc->child(P_COMMUNICATION).child(P_USER);
    string newUserMail;
    string newRole;
    string errText;
    int fail = 0;
    //TODO Superuser nemůže degradovat svůj účet pokud je jediný(poslední) superuser DB triger?
    for (; userNode; userNode = userNode.next_sibling(P_USER))
    {
        newUserMail = userNode.attribute(P_EMAIL).value();
        newRole = userNode.attribute(P_ROLE).value();
        
        if(newRole != P_ROLE_GUEST && newRole != P_ROLE_USER && newRole !=P_ROLE_ADMIN && newRole != P_ROLE_SUPERUSER){
            errText += "<user email=\""+newUserMail+"\" role=\""+newRole+"\"/>";
            fail = ServerException::ROLE;
        }else{
            if(!DAOUsers::getInstance().isMailRegistred(newUserMail)){
                User user;
                user.mail = newUserMail;
                DAOUsers::getInstance().add(user);
            }
             if(DAOUsersAdapters::getInstance().changeConAccount(_adapterId, newUserMail, newRole) != 1){
                    errText += "<user email=\""+newUserMail+"\" role=\""+newRole+"\"/>";
                    fail = ServerException::EMAIL;
             }
        } 
        
    }
    if(fail != 0)
        throw ServerException(fail,errText);
    
    return envelopeResponse(R_TRUE);
}

