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

const string AccountDel::state = "delaccs";
AccountDel::AccountDel(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc)  {
}

AccountDel::~AccountDel() {
}

int AccountDel::getMsgAuthorization() {
    return SUPERUSER;
}

string AccountDel::createResponseMsgOut()
{
    
    pugi::xml_node userNode =  _doc->child(P_COMMUNICATION).child(P_USER);
    string userMail;
    //bool fail=false;
    //string errText;
    
    for (; userNode; userNode = userNode.next_sibling(P_USER))
    {
        userMail = userNode.attribute(P_EMAIL).value();
        
        User user = DAOUsers::getInstance().getUserByID(_userId);
        
        if(userMail != user.mail)
            if(DAOUsersAdapters::getInstance().delConAccount(_gatewayId, userMail) == 0){
                //fail = true;
                //errText += "<user email=\""+userMail+"\"/>";
            }
    }
    
    return getXMLreply(R_TRUE);
}



