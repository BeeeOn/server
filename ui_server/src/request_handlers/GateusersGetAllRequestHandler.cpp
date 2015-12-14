#include <string.h>

#include "GateusersGetAllRequestHandler.h"
#include "../DAO/DAOUsersGateways.h"


using namespace std;

const string GateusersGetAllRequestHandler::state = "getall";
GateusersGetAllRequestHandler::GateusersGetAllRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOUsersGateways & daoUsersGateways, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoUsersGateways(daoUsersGateways)
{
}

GateusersGetAllRequestHandler::~GateusersGetAllRequestHandler() 
{
}

int GateusersGetAllRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

string GateusersGetAllRequestHandler::createResponseMsgOut()
{ 
    return getDataXMLreply(proto::replyAccounts, _daoUsersGateways.getXMLconAccounts(_gatewayId));
}
