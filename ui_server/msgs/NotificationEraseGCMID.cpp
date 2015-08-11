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

const std::string NotificationEraseGCMID::state = "delgcmid";

NotificationEraseGCMID::NotificationEraseGCMID(pugi::xml_document* doc): IMsgInFreeAccess(doc) {

}

NotificationEraseGCMID::~NotificationEraseGCMID() {
}

int NotificationEraseGCMID::getMsgAuthorization() {
    return EVERYONE;
}

string NotificationEraseGCMID::createResponseMsgOut() {
    string gcmid = _doc->child(P_COMMUNICATION).attribute(P_GOOGLE_CLOUD_MID).value();
    
    int uid = _doc->child(P_COMMUNICATION).attribute(P_USER_ID).as_int(-1);
    DAOMobileDevices::getInstance().deletepushNotification(uid, gcmid);
    //DAOMobileDevices::getInstance().delGCMId(uid, gcmid);
    
    return getXMLreply(R_TRUE);
}

