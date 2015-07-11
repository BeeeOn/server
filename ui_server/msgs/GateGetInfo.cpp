/* 
 * File:   GateGetInfo.cpp
 * Author: Pavel
 * 
 * Created on 9. července 2015, 13:37
 */

#include "GateGetInfo.h"
#include "../../DAO/DAOAdapters.h"
#include "IMsgIn.h"


using namespace std;

const string GateGetInfo::state = "getgateinfo";

GateGetInfo::GateGetInfo(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

GateGetInfo::~GateGetInfo() {
}

int GateGetInfo::getMsgAuthorization() {
    return GUEST;
}

string GateGetInfo::createResponseMsgOut()
{ 
    GateInfo gateInfo = DAOAdapters::getInstance().getGateInfo(_adapterId);
    _outputMainNode.append_attribute(P_ADAPTER_ID) = gateInfo.id.c_str();
    _outputMainNode.append_attribute(P_ADAPTER_NAME) = gateInfo.name.c_str();
    _outputMainNode.append_attribute(P_ROLE) = _role.c_str();
    _outputMainNode.append_attribute(P_ADAPTER_N_FACILITIES) = gateInfo.nFacilities;
    _outputMainNode.append_attribute(P_ADAPTER_N_USERS) = gateInfo.nUsers;
    _outputMainNode.append_attribute(P_ADAPTER_IP) = gateInfo.ip;
    _outputMainNode.append_attribute(P_ADAPTER_VERSION) = gateInfo.version;
    _outputMainNode.append_attribute(P_TIME_UTC) = gateInfo.timezone;
    return getXMLreply(R_GATEINFO);
}
