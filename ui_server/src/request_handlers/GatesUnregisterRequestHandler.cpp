#include "GatesUnregisterRequestHandler.h"
#include "../DAO/DAOGateways.h"
#include "../DAO/DAOUsers.h"
#include "Config.h"
#include "DAO/DAOUsersGateways.h"

using namespace std;

const string GatesUnregisterRequestHandler::state = "unregister";

GatesUnregisterRequestHandler::GatesUnregisterRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOGateways & daoGateways, DAOUsersGateways & daoUsersGateways, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoUsersGateways(daoUsersGateways),
        _daoGateways(daoGateways)
{
    _gatewayId = _doc->child(proto::requestNode).child(proto::gatewayNode).attribute(proto::gatewayIdAttr).as_ullong(0);
}

GatesUnregisterRequestHandler::~GatesUnregisterRequestHandler()
{
}

int GatesUnregisterRequestHandler::getMsgAuthorization()
{
    return permissions::guest;
}

string GatesUnregisterRequestHandler::createResponseMsgOut()
{
    if(_role != proto::roleOwnerAttr)
    {
        if(_daoUsersGateways.delConAccount(_gatewayId, _userId) == 0)
        {
            return getNegativeXMLReply(ServerException::ROLE);
        }
    }
    else
    {
        if(_daoGateways.remove(_gatewayId) == 0)
        {
            return getNegativeXMLReply(ServerException::ROLE);
        }
    }
    return getXMLreply(proto::replyTrue);
    
}


