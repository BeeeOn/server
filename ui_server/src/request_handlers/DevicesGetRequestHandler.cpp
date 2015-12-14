#include "DevicesGetRequestHandler.h"
#include "DAO/DAODevices.h"
#include "DAO/DAOLocations.h"

#include <vector>

using namespace std;

const string DevicesGetRequestHandler::state = "get";

DevicesGetRequestHandler::DevicesGetRequestHandler(shared_ptr<pugi::xml_document> doc, DAODevices &daoDevices, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginRequired(doc, sessionTable),
        _daoDevices(daoDevices)
{
}

DevicesGetRequestHandler::~DevicesGetRequestHandler(void)
{
}

string DevicesGetRequestHandler::createResponseMsgOut()
{
    gatewayId_64b gateId;
    uint deviceId;
    
    vector<gatewayId_64b> gatesVec;
    vector<deviceId_64b> devicesVec;
    
    pugi::xml_node deviceNode = _doc->child(proto::requestNode).child(proto::deviceNode);
    
    for (; deviceNode; deviceNode = deviceNode.next_sibling(proto::deviceNode))
    {
        gateId = deviceNode.attribute(proto::deviceGateIdAttr).as_ullong(0);
        deviceId = deviceNode.attribute(proto::deviceIdAttr).as_ullong(0);
        
        gatesVec.push_back(gateId);
        devicesVec.push_back(deviceId); 
    }
        
    return getDataXMLreply(proto::replyDevices, _daoDevices.getXMLdevices(_userId, gatesVec, devicesVec));
}