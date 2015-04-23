/* 
 * File:   MsgInSetGCMID.h
 * Author: pavel
 *
 * Created on 12. prosinec 2014, 9:59
 */

#ifndef MSGINSETGCMID_H
#define	MSGINSETGCMID_H

#include "IMsgInLoginRequired.h"
#include "IMsgIn.h"

class MsgInSetGCMID:public IMsgInLoginRequired {
public:
    MsgInSetGCMID(char* msg, pugi::xml_document* doc);
    virtual ~MsgInSetGCMID();
    

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();

    static const std::string state;
private:

};

#endif	/* MSGINSETGCMID_H */

