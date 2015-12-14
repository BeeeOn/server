#include "GatesStartListenRequestHandler.h"
#include "Config.h"
#include "AdaServerCommunicator.h"

using namespace std;
const std::string GatesStartListenRequestHandler::state = "startlisten";

GatesStartListenRequestHandler::GatesStartListenRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, AdaServerCommunicator & adaServerCom, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _adaServerCom(adaServerCom)
{
    _gatewayId = _doc->child(proto::requestNode).child(proto::gatewayNode).attribute(proto::gatewayIdAttr).as_ullong(0);
}

GatesStartListenRequestHandler::~GatesStartListenRequestHandler()
{
}

int GatesStartListenRequestHandler::getMsgAuthorization()
{
    return permissions::owner;
}

string GatesStartListenRequestHandler::createResponseMsgOut()
{   
    int result = _adaServerCom.sendGatewayStartListen(_gatewayId);
    
    if(result == 0)
    {
        return getPositiveXMLReply();
    }
    else
    {
        return getNegativeXMLReply(result);
    }
}
