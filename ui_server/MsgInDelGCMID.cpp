/* 
 * File:   MsgInDelGCMID.cpp
 * Author: pavel
 * 
 * Created on 12. prosinec 2014, 9:32
 */

#include "MsgInDelGCMID.h"
#include "../DAO/DAOMobileDevices.h"
#include "DBConnector.h"
#include "IMsgInLoginUnwanted.h"


const std::string MsgInDelGCMID::state = "delgcmid";

MsgInDelGCMID::MsgInDelGCMID(char* msg, pugi::xml_document* doc): IMsgInLoginUnwanted(msg, doc) {

}

MsgInDelGCMID::~MsgInDelGCMID() {
}

string MsgInDelGCMID::createResponseMsgOut() {
    string oldGID = _doc->child(P_COMMUNICATION).attribute(P_GOOGLE_ID).value();
    string gcmid = _doc->child(P_COMMUNICATION).attribute(P_GOOGLE_CLOUD_MID).value();
    
    int uid = _doc->child(P_COMMUNICATION).attribute(P_USER_ID).as_int(-1);
    if(DAOMobileDevices::getInstance().delGCMId(uid, gcmid) == 0)
        ;//zadne duplicatni GCMID se nesmazalo - false?
    
    return envelopeResponse(R_TRUE); 
}

int MsgInDelGCMID::getMsgAuthorization() {
    return EVERYONE;
}

