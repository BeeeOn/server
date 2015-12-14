#include "NotificationsSetReadRequestHandler.h"
#include "IRequestHandlerWithLoginRequired.h"
#include "../DAO/DAONotification.h"

using namespace std;

const std::string NotificationsSetReadRequestHandler::state = "read";

NotificationsSetReadRequestHandler::NotificationsSetReadRequestHandler(shared_ptr<pugi::xml_document> doc, DAONotification & daoNotification, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginRequired(doc, sessionTable),
        _daoNotification(daoNotification)
{
}


NotificationsSetReadRequestHandler::~NotificationsSetReadRequestHandler() 
{
}

int NotificationsSetReadRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

std::string NotificationsSetReadRequestHandler::createResponseMsgOut() 
{
    int notif_id = _doc->child(proto::requestNode).child(proto::notificationNode).attribute(proto::locationTypeAttr).as_int(-1);
    
    if ( _daoNotification.setReaded(notif_id) > 0 )
    {
        return getXMLreply(proto::replyTrue);
    }
    else
    {
        _outputMainNode.append_attribute(proto::errorCodeAttr) = ServerException::NOTIFICATION_ID;
        return getXMLreply(proto::replyFalse);
    }
}
