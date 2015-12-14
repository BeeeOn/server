#include "LocationsUpdateRequestHandler.h"
#include "../lib/pugixml.hpp"
#include "../DAO/DAOLocations.h"
#include "IRequestHandler.h"
using namespace std;

const string LocationsUpdateRequestHandler::state = "update";

LocationsUpdateRequestHandler::LocationsUpdateRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOLocations & daoLocations, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoLocations(daoLocations)
{
}

LocationsUpdateRequestHandler::~LocationsUpdateRequestHandler() 
{
}

int LocationsUpdateRequestHandler::getMsgAuthorization() 
{
    return permissions::admin;
}

string LocationsUpdateRequestHandler::createResponseMsgOut()
{                
        pugi::xml_node locationNode =  _doc->child(proto::requestNode).child(proto::locationNode);
        
        string name;
        int locationId, type;
        
        locationId = locationNode.attribute(proto::locationIdAttr).as_int();
        type = locationNode.attribute(proto::locationTypeAttr).as_int(-1);

        name = locationNode.attribute(proto::locationNameAttr).value();
        if(_daoLocations.update(_gatewayId, locationId, type, name) == 0)
        {
            return getNegativeXMLReply(ServerException::MISSING_ENTITY);
        }
        
        return getXMLreply(proto::replyTrue);
}

