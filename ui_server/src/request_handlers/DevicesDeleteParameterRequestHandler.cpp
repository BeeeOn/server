#include "DevicesDeleteParameterRequestHandler.h"
#include "DAO/DAODevicesParameters.h"
using namespace std;

const string DevicesDeleteParameterRequestHandler::state = "deleteparameter";

DevicesDeleteParameterRequestHandler::DevicesDeleteParameterRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevicesParameters & daoDevPar, SessionsTable & sessionTable)
    :    IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoDevPar(daoDevPar)
{
}

DevicesDeleteParameterRequestHandler::~DevicesDeleteParameterRequestHandler() 
{
}

int DevicesDeleteParameterRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

string DevicesDeleteParameterRequestHandler::createResponseMsgOut()
{
    pugi::xml_node deviceNode =  _doc->child(proto::requestNode).child(proto::deviceNode);
    
    deviceParameter devPar;
    devPar.euid = deviceNode.attribute(proto::deviceIdAttr).as_ullong();
    devPar.gatewayId = _gatewayId;
    devPar.key = deviceNode.attribute(proto::deviceParamKeyAttr).value();
 
    if( _daoDevPar.remove(devPar) != 1 )
    {
        return getNegativeXMLReply(ServerException::UNKNOWN_ERROR);
    }

    return getXMLreply(proto::replyTrue);
}
