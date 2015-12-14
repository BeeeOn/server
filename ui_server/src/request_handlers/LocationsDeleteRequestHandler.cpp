#include "LocationsDeleteRequestHandler.h"
#include "../DAO/DAOLocations.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"

using namespace std;

const string LocationsDeleteRequestHandler::state = "delete";

LocationsDeleteRequestHandler::LocationsDeleteRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOLocations & daoLocations, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoLocations(daoLocations)
{
}


LocationsDeleteRequestHandler::~LocationsDeleteRequestHandler() 
{
}

int LocationsDeleteRequestHandler::getMsgAuthorization() 
{
    return permissions::admin;
}

string LocationsDeleteRequestHandler::createResponseMsgOut()
{
    pugi::xml_node locationNode = _doc->child(proto::requestNode).child(proto::locationNode);    
    string locationId = locationNode.attribute(proto::locationIdAttr).value();    
    _daoLocations.remove(_gatewayId, locationId);
    
    return getXMLreply(proto::replyTrue);
}

