/* 
 * File:   MsgInSetGCMID.cpp
 * Author: pavel
 * 
 * Created on 12. prosinec 2014, 9:59
 */

#include "MsgInSetGCMID.h"
#include "../DAO/DAOMobileDevices.h"

using namespace std;

const std::string MsgInSetGCMID::state = "setgcmid";

MsgInSetGCMID::MsgInSetGCMID(pugi::xml_document* doc):IMsgInLoginRequired(doc) {

}

MsgInSetGCMID::~MsgInSetGCMID() {
}

int MsgInSetGCMID::getMsgAuthorization() {
    return USER;
}

string MsgInSetGCMID::createResponseMsgOut() {
    string gcmid = _doc->child(P_COMMUNICATION).attribute(P_GOOGLE_CLOUD_MID).value();
    //string phoneid = _doc->child(P_COMMUNICATION).attribute(P_PHONE_ID).value();
    
    //string mail = _doc->child(P_COMMUNICATION).attribute(P_EMAIL).value();
   // gcmid =   DBConnector::getInstance().DEBUGexec("select user_id from users where mail='"+mail+"';" );
    
    if(DAOMobileDevices::getInstance().setGCMId(_token, gcmid) == 0)
    {
        _outputMainNode.append_attribute(P_ERRCODE) = ServerException::GCMID;
        return getXMLreply(R_FALSE);
    }
    
    return getXMLreply(R_TRUE); 
}


