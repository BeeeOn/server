#include "DevicesUpdateParameterRequestHandler.h"

using namespace std;

const string DevicesUpdateParameterRequestHandler::state = "updateparameter";


DevicesUpdateParameterRequestHandler::DevicesUpdateParameterRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevicesParameters & daoDevPar, SessionsTable & sessionTable)
    :    IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoDevPar(daoDevPar) 
{
}

DevicesUpdateParameterRequestHandler::~DevicesUpdateParameterRequestHandler() 
{
}


int DevicesUpdateParameterRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

string DevicesUpdateParameterRequestHandler::createResponseMsgOut()
{
    pugi::xml_node deviceNode =  _doc->child(proto::requestNode).child(proto::deviceNode);
    
    deviceParameter devPar;
    devPar.euid = deviceNode.attribute(proto::deviceIdAttr).as_ullong();
    devPar.gatewayId = _gatewayId;
    devPar.key = deviceNode.attribute(proto::deviceParamKeyAttr).value();
    devPar.value = deviceNode.attribute(proto::deviceParamValueAttr).value();
    
    if( !_daoDevPar.update(devPar) )
    {
        return getNegativeXMLReply(ServerException::UNKNOWN_ERROR);
    }

    return getXMLreply(proto::replyTrue);
}