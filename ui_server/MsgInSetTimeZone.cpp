/* 
 * File:   MsgInSetTimeZone.cpp
 * Author: pavel
 * 
 * Created on 1. červenec 2014, 11:23
 */

#include "MsgInSetTimeZone.h"
#include "ServerException.h"


using namespace std;

const string MsgInSetTimeZone::state = "settimezone";

MsgInSetTimeZone::MsgInSetTimeZone(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc)  {
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
        throw ServerException(ServerException::TIMEZONE_UTC);
    }
    if(tz <= P_TIME_MIN || tz >= P_TIME_MAX)
        throw ServerException(ServerException::TIMEZONE_UTC);
    
    DBConnector::getInstance().updateAdaptersTimezone(_adapterId, newTimeZone);
    
    
    return envelopeResponse(R_TRUE);
}