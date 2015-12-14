#include "DevicesGetAllRequestHandler.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"

#include "../DAO/DAODevices.h"

using namespace std;

const string DevicesGetAllRequestHandler::state = "getall";

DevicesGetAllRequestHandler::DevicesGetAllRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevices & daoDevices, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoDevices(daoDevices)
{
}

DevicesGetAllRequestHandler::~DevicesGetAllRequestHandler() 
{
}

int DevicesGetAllRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

string DevicesGetAllRequestHandler::createResponseMsgOut()
{   
    return getDataXMLreply(proto::replyAllDevices, _daoDevices.getXMLAllDevs(_gatewayId));
}
