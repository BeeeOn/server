/* 
 * File:   MsgInGetTimeZone.cpp
 * Author: pavel
 * 
 * Created on 1. červenec 2014, 12:04
 */

#include "MsgInGetTimeZone.h"
#include "../DAO/DAOAdapters.h"

using namespace std;

const string MsgInGetTimeZone::state = "gettimezone";
MsgInGetTimeZone::MsgInGetTimeZone(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc)  {
}

MsgInGetTimeZone::~MsgInGetTimeZone() {
}

int MsgInGetTimeZone::getMsgAuthorization() {
    return GUEST;
}

string MsgInGetTimeZone::createResponseMsgOut()
{                
        string attr = makeXMLattribute("utc",DAOAdapters::getInstance().getTimeZone(_adapterId) );
         return envelopeResponseWithAttributes(R_TIMEZONE, attr);
}