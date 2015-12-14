#ifndef MSGINNOTIFICATIONREAD_H
#define	MSGINNOTIFICATIONREAD_H

#include "IRequestHandlerWithLoginRequired.h" 
#include "../DAO/DAONotification.h"

class NotificationsSetReadRequestHandler 
    :   public IRequestHandlerWithLoginRequired
{
    public:
        NotificationsSetReadRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAONotification & daoNotification, SessionsTable & sessionTable);
        virtual ~NotificationsSetReadRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
    private:
        DAONotification & _daoNotification;

};

#endif	/* MSGINNOTIFICATIONREAD_H */

