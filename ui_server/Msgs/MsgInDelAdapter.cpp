/* 
 * File:   MsgInDelAdapter.cpp
 * Author: Pavel
 * 
 * Created on 7. dubna 2015, 9:09
 */

#include "MsgInDelAdapter.h"
#include "../DAO/DAOAdapters.h"

using namespace std;

const string MsgInDelAdapter::state = "deladapter";

MsgInDelAdapter::MsgInDelAdapter(char* msg, pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(msg, doc) {
}

MsgInDelAdapter::~MsgInDelAdapter() {
}

int MsgInDelAdapter::getMsgAuthorization() {
    return GUEST;
}
//Pokud je uživatel SU a tohle udělá, smaže se všechno, musím poslat ada_server zprávu 
string MsgInDelAdapter::createResponseMsgOut() {
    if(_role != "superuser")
    {
        if (DAOAdapters::getInstance().delUsersAdapter(_adapterId, _userId) > 0)
        {
            return genOutputXMLwithVersionAndState(R_TRUE);
        }
    }
    _outputMainNode.append_attribute(P_ERRCODE) = ServerException::ROLE;
    return genOutputXMLwithVersionAndState(R_FALSE);
}


