#include "DevicesCreateParameterRequestHandler.h"
#include "IRequestHandler.h"
#include "AccountsRegisterRequestHandler.h"
#include "DAO/DAODevicesParameters.h"
#include "ServerException.h"

using namespace std;

const string DevicesCreateParameterRequestHandler::state = "createparameter";

DevicesCreateParameterRequestHandler::DevicesCreateParameterRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevicesParameters & daoDevPar, SessionsTable & sessionTable)
    :    IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoDevPar(daoDevPar)
{
}

DevicesCreateParameterRequestHandler::~DevicesCreateParameterRequestHandler() 
{
}


int DevicesCreateParameterRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

string DevicesCreateParameterRequestHandler::createResponseMsgOut()
{
    pugi::xml_node deviceNode =  _doc->child(proto::requestNode).child(proto::deviceNode);
    
    deviceParameter devPar;
    devPar.euid = deviceNode.attribute(proto::deviceIdAttr).as_ullong();
    devPar.gatewayId = _gatewayId;
    devPar.key = deviceNode.attribute(proto::deviceParamKeyAttr).value();
    devPar.value = deviceNode.attribute(proto::deviceParamValueAttr).value();
    
    if( !_daoDevPar.create(devPar) )
    {
        return getNegativeXMLReply(ServerException::UNKNOWN_ERROR);
    }

    return getXMLreply(proto::replyTrue);
}
