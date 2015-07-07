/* 
 * File:   MsgInAddConAccount.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 14:06
 */

#include "MsgInAddConAccount.h"
#include "IMsgInLoginRequired.h"

const std::string MsgInAddConAccount::state = "addconaccount";

MsgInAddConAccount::MsgInAddConAccount(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}

MsgInAddConAccount::~MsgInAddConAccount() {
}

IMsgOut* MsgInAddConAccount::createResponseMsgOut()
{
    if( !isComIdValid())
        return new MsgOutReSign();
    
    pugi::xml_node userNode =  _doc->child("communication").child("user");
    string newUserMail;
    string newRole;
    
    for (; userNode; userNode = userNode.next_sibling("user"))
    {
        newUserMail = userNode.attribute("email").value();
        newRole = userNode.attribute("role").value();
        //if(newRole != "guest" && newRole != "user" && newRole !="admin" && newRole != "superuser")
        //TODO castecny fail
        DBConnector::getInstance().addConAccount(_adapterId, newUserMail, newRole);
    }
    //TODO err code 13
    return new MsgOutTrue(_comId, _state);
}
