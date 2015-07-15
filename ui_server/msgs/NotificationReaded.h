/* 
 * File:   MsgInNotificationRead.h
 * Author: Pavel
 *
 * Created on 23. dubna 2015, 13:08
 */

#ifndef MSGINNOTIFICATIONREAD_H
#define	MSGINNOTIFICATIONREAD_H

#include "IMsgInLoginRequired.h" 
class NotificationReaded : public IMsgInLoginRequired{
public:
    NotificationReaded(pugi::xml_document* doc);
    virtual ~NotificationReaded();
    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();

    static const std::string state;
private:

};

#endif	/* MSGINNOTIFICATIONREAD_H */

