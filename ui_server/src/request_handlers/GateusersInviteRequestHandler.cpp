#include "GateusersInviteRequestHandler.h"
#include "../DAO/DAOUsersGateways.h"

using namespace std;
const string GateusersInviteRequestHandler::state = "invite";

GateusersInviteRequestHandler::GateusersInviteRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOUsersGateways & daoUsersGateways, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoUsersGateways(daoUsersGateways)
{
}

GateusersInviteRequestHandler::~GateusersInviteRequestHandler()
{
}

int GateusersInviteRequestHandler::getMsgAuthorization()
{
    return permissions::owner;
}

string GateusersInviteRequestHandler::createResponseMsgOut()
{    
    pugi::xml_node userNode =  _doc->child(proto::requestNode).child(proto::userNode);
    
    string newUser = userNode.attribute(proto::userEmailAttr).value();
    string newRole = userNode.attribute(proto::userRoleAttr).value();
    
    if(!isRoleValid(newRole))
    {
        return getNegativeXMLReply(ServerException::ROLE);
    }
    else if(_daoUsersGateways.addConAccount(_gatewayId, newUser, newRole) != 1)
    {
        return getNegativeXMLReply(ServerException::USER_ID);
    }        
    
    return getXMLreply(proto::replyTrue);
}
