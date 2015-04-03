/* 
 * File:   MsgInSetGCMID.cpp
 * Author: pavel
 * 
 * Created on 12. prosinec 2014, 9:59
 */

#include "MsgInSetGCMID.h"
#include "../DAO/DAOMobileDevices.h"


const std::string MsgInSetGCMID::state = "setgcmid";

MsgInSetGCMID::MsgInSetGCMID(char* msg, pugi::xml_document* doc):IMsgInLoginRequired(msg, doc) {

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
        throw ServerException(ServerException::GCMID);
    
    return envelopeResponse(R_TRUE); 
}


