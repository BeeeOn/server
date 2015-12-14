#include "GatesUpdateRequestHandler.h"
#include "IRequestHandler.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "../../DAO/DAOUsers.h"
#include <math.h>
using namespace std;

const string GatesUpdateRequestHandler::state = "update";

GatesUpdateRequestHandler::GatesUpdateRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOGateways & daoGateways, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoGateways(daoGateways)
{
    _gatewayId = _doc->child(proto::requestNode).child(proto::gatewayNode).attribute(proto::gatewayIdAttr).as_ullong(0);
}

GatesUpdateRequestHandler::~GatesUpdateRequestHandler() 
{
}

int GatesUpdateRequestHandler::getMsgAuthorization() 
{
    return permissions::admin;
}

string GatesUpdateRequestHandler::createResponseMsgOut()
{                    
    pugi::xml_node gateNode =  _doc->child(proto::requestNode).child(proto::gatewayNode);
        
    Gate gate;
    
    gate.id = _gatewayId;
    gate.timezone = gateNode.attribute(proto::gatewayTimezoneAttr).as_int(999);
    gate.altitude = gateNode.attribute(proto::gatewayAltitudeAttr).as_float(NAN);
    gate.longitude = gateNode.attribute(proto::gatewayLongitudeAttr).as_double(NAN);
    gate.latitude = gateNode.attribute(proto::gatewayLatitudeAttr).as_double(NAN);
    
    gate.name = gateNode.attribute(proto::gatewayNameAttr).value();
    
    _daoGateways.update(gate);
    
    return getXMLreply(proto::replyTrue);
}
