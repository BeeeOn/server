#include "GatesGetAllRequestHandler.h"

using namespace std;

const std::string GatesGetAllRequestHandler::state = "getall";

GatesGetAllRequestHandler::GatesGetAllRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginRequired(doc, sessionTable),
        _daoUsers(daoUsers)
{
}

GatesGetAllRequestHandler::~GatesGetAllRequestHandler(void)
{
}

string GatesGetAllRequestHandler::createResponseMsgOut()
{
    return getDataXMLreply(proto::replyGates, _daoUsers.getXMLusersGates(_userId));
}