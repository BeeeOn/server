#include "DevicesGetNewRequestHandler.h"
#include "../DAO/DAODevices.h"

using namespace std;

const string DevicesGetNewRequestHandler::state = "getnew";

DevicesGetNewRequestHandler::DevicesGetNewRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevices & daoDevices, SessionsTable & sessionTable) 
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoDevices(daoDevices)
{
}

DevicesGetNewRequestHandler::~DevicesGetNewRequestHandler() 
{
}

int DevicesGetNewRequestHandler::getMsgAuthorization() 
{
    return permissions::owner;
}

string DevicesGetNewRequestHandler::createResponseMsgOut() 
{    
     return getDataXMLreply(proto::replyDevices, _daoDevices.getXMLNewDevices(_gatewayId));
}

