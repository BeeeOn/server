#include "LocationsGetAllRequestHandler.h"
#include "../DAO/DAOLocations.h"

using namespace std;

const string LocationsGetAllRequestHandler::state = "getall";

LocationsGetAllRequestHandler::LocationsGetAllRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOLocations & daoLocations, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoLocations(daoLocations)
{
}

LocationsGetAllRequestHandler::~LocationsGetAllRequestHandler() 
{
}

int LocationsGetAllRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

string LocationsGetAllRequestHandler::createResponseMsgOut()
{                        
    return getDataXMLreply(proto::replyLocations, _daoLocations.getXMLrooms(_gatewayId));
}