#ifndef MSGINGETNOTIFICATION_H
#define	MSGINGETNOTIFICATION_H

#include "IRequestHandlerWithLoginRequired.h" 
#include "DAO/DAONotification.h"

class NotificationsGetRequestHandler 
    :   public IRequestHandlerWithLoginRequired
{
    public:
        NotificationsGetRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAONotification & daoNotification, SessionsTable & sessionTable);
        virtual ~NotificationsGetRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
    private:
        DAONotification & _daoNotification;
};

#endif	/* MSGINGETNOTIFICATION_H */

