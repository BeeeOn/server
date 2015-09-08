/* 
 * File:   UserLogout.h
 * Author: Pavel
 *
 * Created on 9. července 2015, 14:07
 */

#ifndef USERLOGOUT_H
#define	USERLOGOUT_H

#include "IMsgInLoginRequired.h"


class UserLogout : public IMsgInLoginRequired {
public:
    UserLogout(pugi::xml_document* doc);;
    virtual ~UserLogout();
    virtual std::string createResponseMsgOut();
    virtual int getMsgAuthorization();
    static const std::string state;

};

#endif	/* USERLOGOUT_H */

