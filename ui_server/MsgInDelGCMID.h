/* 
 * File:   MsgInDelGCMID.h
 * Author: pavel
 *
 * Created on 12. prosinec 2014, 9:32
 */

#ifndef MSGINDELGCMID_H
#define	MSGINDELGCMID_H

#include "IMsgInLoginRequired.h"


class MsgInDelGCMID : public IMsgInLoginRequired{
public:
    MsgInDelGCMID(char* msg, pugi::xml_document* doc);
    virtual ~MsgInDelGCMID();
    

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();
    static const std::string state;


    
private:

};

#endif	/* MSGINDELGCMID_H */

