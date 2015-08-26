/* 
 * File:   MsgInDelGCMID.cpp
 * Author: pavel
 * 
 * Created on 12. prosinec 2014, 9:32
 */

#include "NotificationEraseGCMID.h"
#include "../DAO/DAOMobileDevices.h"
#include "DBConnector.h"
#include "IMsgInLoginUnwanted.h"

using namespace std;

const std::string NotificationEraseGCMID::state = "deletegcmid";

NotificationEraseGCMID::NotificationEraseGCMID(pugi::xml_document* doc): IMsgInFreeAccess(doc) {

}

NotificationEraseGCMID::~NotificationEraseGCMID() {
}

int NotificationEraseGCMID::getMsgAuthorization() {
    return permissions::everyone;
}

string NotificationEraseGCMID::createResponseMsgOut() {
    string gcmid = _doc->child(proto::communicationNode).attribute(proto::googleCloudMessagingIdAttr).value();
    
    int uid = _doc->child(proto::communicationNode).child(proto::userNode).attribute(proto::userIdAttr).as_int(-1);
    DAOMobileDevices::getInstance().deletepushNotification(uid, gcmid);
    //DAOMobileDevices::getInstance().delGCMId(uid, gcmid);
    
    return getXMLreply(proto::replyTrue);
}

