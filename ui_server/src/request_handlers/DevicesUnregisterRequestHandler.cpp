#include "DevicesUnregisterRequestHandler.h"
#include "Config.h"
using namespace std;
const string DevicesUnregitersRequestHandler::state = "unregister";
DevicesUnregitersRequestHandler::DevicesUnregitersRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, AdaServerCommunicator & adaServerCom, SessionsTable & sessionTable) 
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _adaServerCom(adaServerCom)
{
}

DevicesUnregitersRequestHandler::~DevicesUnregitersRequestHandler()
{
}

int DevicesUnregitersRequestHandler::getMsgAuthorization() 
{
    return permissions::owner;
}

string DevicesUnregitersRequestHandler::createResponseMsgOut()
{    
    pugi::xml_node deviceNode =  _doc->child(proto::requestNode).child(proto::deviceNode);
    deviceId_64b deviceId = deviceNode.attribute(proto::deviceIdAttr).as_ullong(0);
    int result = _adaServerCom.sendUnregisterDevice(_gatewayId, deviceId);
    
    if(result == 0)
        return getPositiveXMLReply();
    else
        return getNegativeXMLReply(result);    
}