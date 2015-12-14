#include "DevicesUpdateRequestHandler.h"
#include "ServerException.h"
#include "../DAO/DAODevices.h"
using namespace std;
const string DevicesUpdateRequestHandler::state = "update";

DevicesUpdateRequestHandler::DevicesUpdateRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevices & daoDevices, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoDevices(daoDevices)
{
}

DevicesUpdateRequestHandler::~DevicesUpdateRequestHandler() 
{
}

int DevicesUpdateRequestHandler::getMsgAuthorization() 
{
    return permissions::admin;
}

string DevicesUpdateRequestHandler::createResponseMsgOut()
{        
    pugi::xml_node deviceNode =  _doc->child(proto::requestNode).child(proto::deviceNode);
    
    string id = deviceNode.attribute(proto::deviceIdAttr).value();
    string init = "1";
    string locationId = deviceNode.attribute(proto::deviceLocationIdAttr).value();
    string name = deviceNode.attribute(proto::deviceNameAttr).value();

    if(init != "" || locationId != "" || name != "")
        _daoDevices.updateFacility(_gatewayId, id, init, locationId, name);
    
    return getXMLreply(proto::replyTrue);
}