#include "DevicesSetStateRequestHandler.h"
#include "Config.h"
#include "AdaServerCommunicator.h"

using namespace std;

const string DevicesSetStateRequestHandler::state = "setstate";
DevicesSetStateRequestHandler::DevicesSetStateRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, AdaServerCommunicator & adaServerCom, SessionsTable & sessionTable) 
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _adaServerCom(adaServerCom)
{
}

DevicesSetStateRequestHandler::~DevicesSetStateRequestHandler() 
{
}

int DevicesSetStateRequestHandler::getMsgAuthorization() 
{
    return permissions::user;
}

string DevicesSetStateRequestHandler::createResponseMsgOut()
{  
    pugi::xml_node comNode =  _doc->child(proto::requestNode);
    pugi::xml_node deviceNode =  comNode.child(proto::deviceNode);
    deviceId_64b deviceId = deviceNode.attribute(proto::deviceIdAttr).as_ullong(0);
    int deviceType = deviceNode.attribute(proto::deviceModuleIdAttr).as_int(-1);
    string newValue = deviceNode.attribute(proto::moduleValueIdAttr).value();

    int result = _adaServerCom.sendSetState(_gatewayId, deviceId, deviceType, newValue);
    
    if(result == 0)
        return getPositiveXMLReply();
    else
        return getNegativeXMLReply(result);        
}
