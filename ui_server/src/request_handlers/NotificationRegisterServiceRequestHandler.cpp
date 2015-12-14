#include "NotificationRegisterServiceRequestHandler.h"
#include "../DAO/DAOPushNotificationService.h"

using namespace std;

const std::string NotificationRegisterServiceRequestHandler::state = "registerservice";

NotificationRegisterServiceRequestHandler::NotificationRegisterServiceRequestHandler(shared_ptr<pugi::xml_document> doc, DAOPushNotificationService & daoPushNotificationService, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginRequired(doc, sessionTable),
        _daoPushNotificationService(daoPushNotificationService)
{
}

NotificationRegisterServiceRequestHandler::~NotificationRegisterServiceRequestHandler() 
{
}

int NotificationRegisterServiceRequestHandler::getMsgAuthorization() 
{
    return permissions::user;
}

string NotificationRegisterServiceRequestHandler::createResponseMsgOut() 
{
    pugi::xml_node serviceNode = _doc->child(proto::requestNode).child(proto::pushNotificationServiceNode);
    string serviceName = serviceNode.attribute(proto::pushNotificationServiceNameAttr).value();
    string serviceId = serviceNode.attribute(proto::pushNotificationServiceIdAttr).value();
    
    if(_daoPushNotificationService.insertPushNotification(_userId, serviceId) == 0)
    {
        return getNegativeXMLReply(ServerException::GCMID);
    }
    return getXMLreply(proto::replyTrue); 
}


