#include "LocationsAddRequestHandler.h"
#include "../DAO/DAOLocations.h"
#include "ServerException.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"

using namespace std;

const std::string LocationsAddRequestHandler::state = "create";

LocationsAddRequestHandler::LocationsAddRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOLocations & daoLocations, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoLocations(daoLocations)
{
}

LocationsAddRequestHandler::~LocationsAddRequestHandler() 
{
}

int LocationsAddRequestHandler::getMsgAuthorization() 
{
    return permissions::admin;
}

string LocationsAddRequestHandler::createResponseMsgOut()
{
    pugi::xml_node locationNode = _doc->child(proto::requestNode).child(proto::locationNode);

    int locType = locationNode.attribute(proto::locationTypeAttr).as_int(-1);
    string locName = locationNode.attribute(proto::locationNameAttr).value();
    int newLocId = _daoLocations.insertNewRoom(_gatewayId, locType, locName);
    
    pugi::xml_node newLocationNode = _outputMainNode.append_child();
    newLocationNode.set_name(proto::locationNode);
    newLocationNode.append_attribute(proto::locationIdAttr) = newLocId;
    newLocationNode.append_attribute(proto::locationTypeAttr) = locType;
    newLocationNode.append_attribute(proto::locationNameAttr) = locName.c_str();
    return getXMLreply(proto::replyLocationCreated);
}
