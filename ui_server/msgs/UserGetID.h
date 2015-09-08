/* 
 * File:   MsgInGetUserID.h
 * Author: pavel
 *
 * Created on 17. březen 2015, 18:04
 */

#ifndef MSGINGETUSERID_H
#define	MSGINGETUSERID_H
#include "IMsgIn.h"
#include "IMsgInLoginRequired.h"


class UserGetID : public IMsgInLoginRequired {
public:
    UserGetID(pugi::xml_document* doc);
    virtual ~UserGetID();
    virtual std::string createResponseMsgOut();
    virtual int getMsgAuthorization();
    static const std::string state;
    
};

#endif	/* MSGINGETUSERID_H */

