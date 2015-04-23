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


class MsgInGetUserID : public IMsgInLoginRequired {
public:
    MsgInGetUserID(char* msg, pugi::xml_document* doc);
    virtual ~MsgInGetUserID();
    virtual std::string createResponseMsgOut();
    virtual int getMsgAuthorization();
    static const std::string state;
    
};

#endif	/* MSGINGETUSERID_H */

