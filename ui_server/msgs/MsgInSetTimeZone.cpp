/* 
 * File:   MsgInSetTimeZone.cpp
 * Author: pavel
 * 
 * Created on 1. červenec 2014, 11:23
 */

#include "MsgInSetTimeZone.h"
#include "ServerException.h"
#include "../DAO/DAOAdapters.h"

using namespace std;

const string MsgInSetTimeZone::state = "settimezone";

MsgInSetTimeZone::MsgInSetTimeZone(pugi::xml_document* doc): IMsgInLoginRequired(doc)  {
}


MsgInSetTimeZone::~MsgInSetTimeZone() {
}

int MsgInSetTimeZone::getMsgAuthorization() {
    return ADMIN;
}

string MsgInSetTimeZone::createResponseMsgOut()
{                
    string newTimeZone = _doc->child(P_COMMUNICATION).attribute(P_TIME_UTC).value();
    int tz;
    try{
        tz = stoi(newTimeZone);
    }catch(...){
        _outputMainNode.append_attribute(P_ERRCODE) = ServerException::TIMEZONE_UTC;
        return genOutputXMLwithVersionAndState(R_FALSE);
    }
    if(tz <= P_TIME_MIN || tz >= P_TIME_MAX)
    {
        _outputMainNode.append_attribute(P_ERRCODE) = ServerException::TIMEZONE_UTC;
        return genOutputXMLwithVersionAndState(R_FALSE);
    }
    
    DAOAdapters::getInstance().updateAdaptersTimezone(_adapterId, newTimeZone);
    
    return genOutputXMLwithVersionAndState(R_TRUE);
}