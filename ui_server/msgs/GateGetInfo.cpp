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
    return permissions::admin;
}

string GateGetInfo::createResponseMsgOut()
{ 
    GateInfo gateInfo = DAOAdapters::getInstance().getGateInfo(_gatewayId);
    
    if(gateInfo.id < 0){
        return getNegativeXMLReply(ServerException::ADAPTER_ID);
    }
    
    pugi::xml_node gateNode = _outputMainNode.append_child();
    gateNode.set_name(proto::adapterNode);
    
    gateNode.append_attribute(proto::gatewayIdAttr) = gateInfo.id;
    gateNode.append_attribute(proto::gatewayNameAttr) = gateInfo.name.c_str();
    gateNode.append_attribute(proto::gatewayUsersRole) = _role.c_str();
    gateNode.append_attribute(proto::gatewayInfoNdevicesAttr) = gateInfo.nFacilities;
    gateNode.append_attribute(proto::gatewayInfoNusersAttr) = gateInfo.nUsers;
    gateNode.append_attribute(proto::gatewayIpAttr) = gateInfo.ip;
    gateNode.append_attribute(proto::gatewayVersionAttr) = gateInfo.version;
    gateNode.append_attribute(proto::gatewayUtcAttr) = gateInfo.timezone;
    
    
    return getXMLreply(proto::replyGateInfo);
}
