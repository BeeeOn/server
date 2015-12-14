#include "GateusersRemoveRequestHandler.h"
#include "../DAO/DAOUsersGateways.h"
#include "../DAO/DAOUsers.h"

using namespace std;

const string GateusersRemoveRequestHandler::state = "remove";
GateusersRemoveRequestHandler::GateusersRemoveRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOUsersGateways & daoUsersGateways, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoUsersGateways(daoUsersGateways)
{
}

GateusersRemoveRequestHandler::~GateusersRemoveRequestHandler()
{
}

int GateusersRemoveRequestHandler::getMsgAuthorization()
{
    return permissions::owner;
}

string GateusersRemoveRequestHandler::createResponseMsgOut()
{
    pugi::xml_node userNode =  _doc->child(proto::requestNode).child(proto::userNode);
    
    int userIdToDelete = userNode.attribute(proto::userIdAttr).as_int(-1);

    //user cannot delete himself
    if(userIdToDelete == _userId)
        return getNegativeXMLReply(ServerException::FORBIDDEN_ACTION);
        
    if(_daoUsersGateways.delConAccount(_gatewayId, userIdToDelete) == 0)
        return getNegativeXMLReply(ServerException::USER_ID);
    
    return getXMLreply(proto::replyTrue);
}



