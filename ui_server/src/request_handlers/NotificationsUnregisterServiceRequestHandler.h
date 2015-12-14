#ifndef MSGINDELGCMID_H
#define	MSGINDELGCMID_H

#include "IRequestHandlerWithLoginRequired.h"
#include "IRequestHandlerWithoutUac.h"
#include "DAO/DAOPushNotificationService.h"

class NotificationsUnregisterServiceRequestHandler 
    :   public IRequestHandlerWithoutUac
{
    public:
        NotificationsUnregisterServiceRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOPushNotificationService & daoPushNotification);
        virtual ~NotificationsUnregisterServiceRequestHandler();    
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;

    private:
        DAOPushNotificationService & _daoPushNotificationService;
};

#endif	/* MSGINDELGCMID_H */

