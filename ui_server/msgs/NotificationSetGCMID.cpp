/* 
 * File:   MsgInSetGCMID.cpp
 * Author: pavel
 * 
 * Created on 12. prosinec 2014, 9:59
 */

#include "NotificationSetGCMID.h"
#include "../DAO/DAOMobileDevices.h"

using namespace std;

const std::string NotificationSetGCMID::state = "setgcmid";

NotificationSetGCMID::NotificationSetGCMID(pugi::xml_document* doc):IMsgInLoginRequired(doc) {

}

NotificationSetGCMID::~NotificationSetGCMID() {
}

int NotificationSetGCMID::getMsgAuthorization() {
    return permissions::user;
}

string NotificationSetGCMID::createResponseMsgOut() {
    string gcmid = _doc->child(proto::communicationNode).attribute(proto::googleCloudMessagingIdAttr).value();
    //string phoneid = _doc->child(P_COMMUNICATION).attribute(P_PHONE_ID).value();
    
    //string mail = _doc->child(P_COMMUNICATION).attribute(proto::emailAttr).value();
   // gcmid =   DBConnector::getInstance().DEBUGexec("select user_id from users where mail='"+mail+"';" );
    
//    if(DAOMobileDevices::getInstance().setGCMId(_token, gcmid) == 0)
//    {
//        _outputMainNode.append_attribute(proto::errorCodeAttr) = ServerException::GCMID;
//        return getXMLreply(proto::replyFalse);
//    }
//    
    
    if(DAOMobileDevices::getInstance().insertPushNotification(_userId, gcmid) == 0)
    {
        return getNegativeXMLReply(ServerException::GCMID);
    }
    return getXMLreply(proto::replyTrue); 
}


