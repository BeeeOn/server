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
MsgInGetTimeZone::MsgInGetTimeZone(pugi::xml_document* doc): IMsgInLoginRequired(doc)  {
}

MsgInGetTimeZone::~MsgInGetTimeZone() {
}

int MsgInGetTimeZone::getMsgAuthorization() {
    return GUEST;
}

string MsgInGetTimeZone::createResponseMsgOut()
{                
    //string attr = makeXMLattribute("utc",DAOAdapters::getInstance().getTimeZone(_adapterId) );
    //return envelopeResponseWithAttributes(R_TIMEZONE, attr);
    
    string tz = DAOAdapters::getInstance().getTimeZone(_adapterId);
    _outputMainNode.append_attribute(P_TIME_UTC) = tz.c_str();
    return genOutputXMLwithVersionAndState(R_TIMEZONE);
}