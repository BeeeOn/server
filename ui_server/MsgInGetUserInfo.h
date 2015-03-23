/* 
 * File:   MsgInGetUserInfo.h
 * Author: pavel
 *
 * Created on 21. březen 2015, 18:45
 */

#ifndef MSGINGETUSERINFO_H
#define	MSGINGETUSERINFO_H

#include "IMsgInLoginRequired.h"

class MsgInGetUserInfo: public IMsgInLoginRequired {
public:
    MsgInGetUserInfo(char* msg, pugi::xml_document* doc);;
    virtual ~MsgInGetUserInfo();
    virtual string createResponseMsgOut();
    virtual int getMsgAuthorization();
    static const std::string state;

};

#endif	/* MSGINGETUSERINFO_H */

