/* 
 * File:   MsgInSetAdapter.cpp
 * Author: Pavel
 * 
 * Created on 9. července 2015, 13:21
 */

#include "MsgInSetAdapter.h"
#include "IMsgIn.h"
using namespace std;

const string MsgInSetAdapter::state = "setgate";

MsgInSetAdapter::MsgInSetAdapter(pugi::xml_document* doc): IMsgInLoginRequired(doc)  {
}


MsgInSetAdapter::~MsgInSetAdapter() {
}

int MsgInSetAdapter::getMsgAuthorization() {
    return ADMIN;
}

string MsgInSetAdapter::createResponseMsgOut()
{                     
    string newTimeZone = _doc->child(P_COMMUNICATION).attribute(P_TIME_UTC).value();
    if(newTimeZone != ""){
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
    }
    string newName = _doc->child(P_COMMUNICATION).attribute(P_ADAPTER_NAME).value();
    
    DAOAdapters::getInstance().updateAdapter(_adapterId, newName, newTimeZone);
    
    return genOutputXMLwithVersionAndState(R_TRUE);
}