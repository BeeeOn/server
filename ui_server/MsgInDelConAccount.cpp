/* 
 * File:   MsgInDelConAccount.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 11:32
 */

#include "MsgInDelAccount.h"
#include "../DAO/DAOUsersAdapters.h"


using namespace std;

const string MsgInDelConAccount::state = "delaccs";
MsgInDelConAccount::MsgInDelConAccount(char* msg, pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(msg, doc)  {
}

MsgInDelConAccount::~MsgInDelConAccount() {
}

int MsgInDelConAccount::getMsgAuthorization() {
    return GUEST;
}

string MsgInDelConAccount::createResponseMsgOut()
{
    
    pugi::xml_node userNode =  _doc->child(P_COMMUNICATION).child(P_USER);
    string userMail;
    //bool fail=false;
    string errText;
    
    for (; userNode; userNode = userNode.next_sibling(P_USER))
    {
        userMail = userNode.attribute(P_EMAIL).value();
        if(DAOUsersAdapters::getInstance().delConAccount(_adapterId, userMail) ==0){
            //fail = true;
            //errText += "<user email=\""+userMail+"\"/>";
        }
    }
    
   // if(fail)
    //    return new MsgOutFalse(_comId, _state,ServerException::EMAIL,errText);
    
    return envelopeResponse(R_TRUE);
}



