#include "GatesRegisterRequestHandler.h"
#include "save_custom_writer.h"
#include "Config.h"

using namespace std;

const std::string GatesRegisterRequestHandler::state = "register";

GatesRegisterRequestHandler::GatesRegisterRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOGateways & daoGateway, DAOUsersGateways & daoUsersGateway, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginRequired(doc, sessionTable),
        _daoUsers(daoUsers),
        _daoGateways(daoGateway),
        _daoUsersGateways(daoUsersGateway)
{
}

GatesRegisterRequestHandler::~GatesRegisterRequestHandler()
{
}

string GatesRegisterRequestHandler::createResponseMsgOut()
{

    pugi::xml_node gatewayNode =  _doc->child(proto::requestNode).child(proto::gatewayNode);
    
    Gate gate;
    gate.id = gatewayNode.attribute(proto::gatewayIdAttr).as_ullong(0);
    gate.name = gatewayNode.attribute(proto::gatewayNameAttr).value();
    gate.timezone = gatewayNode.attribute(proto::gatewayTimezoneAttr).as_int(0);
   
    if(_daoGateways.exists(gate.id) == 0)
    {
        return getNegativeXMLReply(ServerException::GATEWAY_ID);
    }
    else if(_daoUsers.isGateParred(_userId, gate.id))
    {
        return getNegativeXMLReply(ServerException::GATEWAY_ACCESSIBLE);
    }
    else
    {
        if(_daoUsersGateways.parGateWithUserIfPossible(gate.id, _userId) == 0)
            return getNegativeXMLReply(ServerException::GATEWAY_TAKEN);
        _daoGateways.update(gate);
    }
    
    return getXMLreply(proto::replyTrue);
}