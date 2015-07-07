/* 
 * File:   MsgInNotificationRead.h
 * Author: Pavel
 *
 * Created on 23. dubna 2015, 13:08
 */

#ifndef MSGINGETNOTIFICATION_H
#define	MSGINGETNOTIFICATION_H

#include "IMsgInLoginRequired.h" 

class MsgInGetNotifications : public IMsgInLoginRequired{
public:
    MsgInGetNotifications(pugi::xml_document* doc);
    virtual ~MsgInGetNotifications();
    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();

    static const std::string state;
private:

};

#endif	/* MSGINGETNOTIFICATION_H */

