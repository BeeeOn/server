#include <string>

#include "DevicesGetParameterRequestHandler.h"
#include "IRequestHandler.h"
#include "DAO/DAODevicesParameters.h"

using namespace std;

const string DevicesGetParameterRequestHandler::state = "getparameter";


DevicesGetParameterRequestHandler::DevicesGetParameterRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevicesParameters & daoDevPar, SessionsTable & sessionTable)
    :    IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoDevPar(daoDevPar)
{
}

DevicesGetParameterRequestHandler::~DevicesGetParameterRequestHandler() 
{
}

int DevicesGetParameterRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

string DevicesGetParameterRequestHandler::createResponseMsgOut()
{
    pugi::xml_node deviceNode =  _doc->child(proto::requestNode).child(proto::deviceNode);
    
    deviceParameter devPar;
    devPar.euid = deviceNode.attribute(proto::deviceIdAttr).as_ullong();
    devPar.gatewayId = _gatewayId;
    devPar.key = deviceNode.attribute(proto::deviceParamKeyAttr).value();

    deviceParameter devParResult = _daoDevPar.get(devPar);
    
    pugi::xml_node resultDeviceNode = _outputMainNode.append_child();
    resultDeviceNode.set_name(proto::deviceNode);
    resultDeviceNode.append_attribute(proto::deviceIdAttr) = devParResult.euid;
    resultDeviceNode.append_attribute(proto::deviceGateIdAttr) = devParResult.gatewayId;
    resultDeviceNode.append_attribute(proto::deviceParamKeyAttr) = devParResult.key.c_str();
    resultDeviceNode.append_attribute(proto::deviceParamValueAttr) = devParResult.value.c_str();

    return getXMLreply(proto::replyData);
}
