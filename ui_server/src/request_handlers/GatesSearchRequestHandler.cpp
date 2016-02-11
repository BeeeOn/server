#include "GatesSearchRequestHandler.h"
#include "AdaServerCommunicator.h"
#include "lib/pugixml.hpp"
#include "IRequestHandler.h"

using namespace std;
const std::string GatesSearchRequestHandler::state = "search";

GatesSearchRequestHandler::GatesSearchRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, AdaServerCommunicator & adaServerCom, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _adaServerCom(adaServerCom)
{
    _gatewayId = _doc->child(proto::requestNode).child(proto::gatewayNode).attribute(proto::gatewayIdAttr).as_ullong(0);
}


GatesSearchRequestHandler::~GatesSearchRequestHandler() 
{
}

int GatesSearchRequestHandler::getMsgAuthorization()
{
    return permissions::owner;
}

string GatesSearchRequestHandler::createResponseMsgOut()
{   
    string ip = _doc->child(proto::requestNode).child(proto::gatewayNode).attribute(proto::gatewayDeviceIpAttr).value();
    string deviceEuid = _doc->child(proto::requestNode).child(proto::gatewayNode).attribute(proto::gatewayDeviceEuidpAttr).value();
    int result = _adaServerCom.sendGatewayStartSearch(_gatewayId, ip, deviceEuid);
    
    if(result == 0)
    {
        return getPositiveXMLReply();
    }
    else
    {
        return getNegativeXMLReply(result);
    }
}
