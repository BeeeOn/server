#include "GateusersUpdateRequestHandler.h"
#include "../DAO/DAOUsers.h"
#include "../DAO/DAOUsersGateways.h"

using namespace std;

const string GateusersUpdateRequestHandler::state = "updateaccess";

GateusersUpdateRequestHandler::GateusersUpdateRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOUsersGateways & daoUsersGateways, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoUsersGateways(daoUsersGateways)
{
}

GateusersUpdateRequestHandler::~GateusersUpdateRequestHandler()
{
}

int GateusersUpdateRequestHandler::getMsgAuthorization()
{
    return permissions::owner;
}

string GateusersUpdateRequestHandler::createResponseMsgOut()
{
    int fail = 0;
    
    pugi::xml_node userNode =  _doc->child(proto::requestNode).child(proto::userNode);
    int newUserId = userNode.attribute(proto::userIdAttr).as_int(-1);
    string newRole = userNode.attribute(proto::userRoleAttr).value();

    //check role validity
    if(!isRoleValid(newRole))
    {
        fail = ServerException::ROLE;
    }
    else
    {
        if(newUserId != _userId)
        //change users role
            if(_daoUsersGateways.changeConAccount(_gatewayId, newUserId, newRole) != 1)
            {
                   fail = ServerException::USER_ID;
            }
    } 
    
    if(fail != 0)
        return getNegativeXMLReply(fail);
    
    return getXMLreply(proto::replyTrue);
}

