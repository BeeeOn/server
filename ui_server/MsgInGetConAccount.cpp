/* 
 * File:   MsgInGetConAccount.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 11:23
 */

#include "MsgInGetAccount.h"
#include "DBConnector.h"


using namespace std;

const string MsgInGetConAccount::state = "getaccs";
MsgInGetConAccount::MsgInGetConAccount(char* msg, pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(msg, doc)  {
}

MsgInGetConAccount::~MsgInGetConAccount() {
}

int MsgInGetConAccount::getMsgAuthorization() {
    return SUPERUSER;
}

string MsgInGetConAccount::createResponseMsgOut()
{                
    return envelopeResponse(R_ACCOUNTS, DBConnector::getInstance().getXMLconAccounts(_adapterId));
}
