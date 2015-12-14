#include "NotificationsUnregisterServiceRequestHandler.h"
#include "../DAO/DAOPushNotificationService.h"

#include "IRequestHandlerWithoutUac.h"

using namespace std;

const std::string NotificationsUnregisterServiceRequestHandler::state = "unregisterservice";

NotificationsUnregisterServiceRequestHandler::NotificationsUnregisterServiceRequestHandler(shared_ptr<pugi::xml_document> doc, DAOPushNotificationService & daoPushNotification)
    :   IRequestHandlerWithoutUac(doc),
        _daoPushNotificationService(daoPushNotification)
{
}

NotificationsUnregisterServiceRequestHandler::~NotificationsUnregisterServiceRequestHandler() 
{
}

int NotificationsUnregisterServiceRequestHandler::getMsgAuthorization() 
{
    return permissions::everyone;
}

string NotificationsUnregisterServiceRequestHandler::createResponseMsgOut() 
{
    pugi::xml_node serviceNode = _doc->child(proto::requestNode).child(proto::pushNotificationServiceNode);
    string serviceId = serviceNode.attribute(proto::pushNotificationServiceIdAttr).value();
    
    int uid = serviceNode.attribute(proto::pushNotificationServiceUserIdAttr).as_int(-1);
    _daoPushNotificationService.deletepushNotification(uid, serviceId);
    
    return getXMLreply(proto::replyTrue);
}

