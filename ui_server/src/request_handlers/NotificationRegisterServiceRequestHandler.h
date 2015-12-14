#ifndef MSGINSETGCMID_H
#define	MSGINSETGCMID_H

#include "IRequestHandlerWithLoginRequired.h"
#include "IRequestHandler.h"
#include "../DAO/DAOPushNotificationService.h"

class NotificationRegisterServiceRequestHandler
    :   public IRequestHandlerWithLoginRequired 
{
    public:
        NotificationRegisterServiceRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOPushNotificationService & daoPushNotificationService, SessionsTable & sessionTable);
        virtual ~NotificationRegisterServiceRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
    private:
        DAOPushNotificationService & _daoPushNotificationService;
};

#endif	/* MSGINSETGCMID_H */

