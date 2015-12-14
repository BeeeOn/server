#include "GatesGetRequestHandler.h"
#include "../../DAO/DAOGateways.h"
#include "../../DAO/DAOUsers.h"
#include "IRequestHandler.h"

using namespace std;

const string GatesGetRequestHandler::state = "get";

GatesGetRequestHandler::GatesGetRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOGateways & daoGateway, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoGateways(daoGateway)
{
    _gatewayId = _doc->child(proto::requestNode).child(proto::gatewayNode).attribute(proto::gatewayIdAttr).as_ullong(0);
}

GatesGetRequestHandler::~GatesGetRequestHandler() 
{
}

int GatesGetRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

string GatesGetRequestHandler::createResponseMsgOut()
{ 
    GateInfo gateInfo = _daoGateways.getGateInfo(_gatewayId);
    
    if(gateInfo.id < 0)
    {
        return getNegativeXMLReply(ServerException::GATEWAY_ID);
    }
    
    pugi::xml_node gateNode = _outputMainNode.append_child();
    gateNode.set_name(proto::gatewayNode);
    
    gateNode.append_attribute(proto::gatewayIdAttr) = gateInfo.id;
    gateNode.append_attribute(proto::gatewayNameAttr) = gateInfo.name.c_str();
    gateNode.append_attribute(proto::gatewayUsersRole) = _role.c_str();
    gateNode.append_attribute(proto::gatewayInfoNdevicesAttr) = gateInfo.nFacilities;
    gateNode.append_attribute(proto::gatewayInfoNusersAttr) = gateInfo.nUsers;
    gateNode.append_attribute(proto::gatewayIpAttr) = gateInfo.ip;
    gateNode.append_attribute(proto::gatewayVersionAttr) = gateInfo.version;
    gateNode.append_attribute(proto::gatewayTimezoneAttr) = gateInfo.timezone;
    gateNode.append_attribute(proto::gatewayInfoOwner) = gateInfo.owner.c_str();
    gateNode.append_attribute(proto::gatewayAltitudeAttr) = gateInfo.altitude;
    gateNode.append_attribute(proto::gatewayStatusAttr) = gateInfo.status.c_str();

    return getXMLreply(proto::replyGateInfo);
}
