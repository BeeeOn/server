/* 
 * File:   MsgInGetConAccount.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 11:23
 */

#include "MsgInGetAccount.h"
#include "../DAO/DAOUsersAdapters.h"


using namespace std;

const string MsgInGetConAccount::state = "getaccs";
MsgInGetConAccount::MsgInGetConAccount(char* msg, pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(msg, doc)  {
}

MsgInGetConAccount::~MsgInGetConAccount() {
}

int MsgInGetConAccount::getMsgAuthorization() {
    return GUEST;
}

string MsgInGetConAccount::createResponseMsgOut()
{                
    return envelopeResponse(R_ACCOUNTS, DAOUsersAdapters::getInstance().getXMLconAccounts(_adapterId));
}
