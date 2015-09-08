/* 
 * File:   MsgInGetUserInfo.h
 * Author: pavel
 *
 * Created on 21. březen 2015, 18:45
 */

#ifndef MSGINGETUSERINFO_H
#define	MSGINGETUSERINFO_H

#include "IMsgInLoginRequired.h"

class UserGetInfo: public IMsgInLoginRequired {
public:
    UserGetInfo(pugi::xml_document* doc);;
    virtual ~UserGetInfo();
    virtual std::string createResponseMsgOut();
    virtual int getMsgAuthorization();
    static const std::string state;

};

#endif	/* MSGINGETUSERINFO_H */

