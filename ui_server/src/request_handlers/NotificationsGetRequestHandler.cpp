#include "NotificationsGetRequestHandler.h"
#include "IRequestHandlerWithLoginRequired.h"
#include "../DAO/DAONotification.h"

const std::string NotificationsGetRequestHandler::state = "getlatest";

NotificationsGetRequestHandler::NotificationsGetRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAONotification & daoNotification, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginRequired(doc, sessionTable),
        _daoNotification(daoNotification)
{
}


NotificationsGetRequestHandler::~NotificationsGetRequestHandler() 
{
}

int NotificationsGetRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

std::string NotificationsGetRequestHandler::createResponseMsgOut() 
{
    return getDataXMLreply(proto::replyNotifications, _daoNotification.getXMLNotifications(_userId));
}
