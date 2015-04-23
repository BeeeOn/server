/* 
 * File:   MsgInDelGCMID.h
 * Author: pavel
 *
 * Created on 12. prosinec 2014, 9:32
 */

#ifndef MSGINDELGCMID_H
#define	MSGINDELGCMID_H

#include "IMsgInLoginRequired.h"
#include "IMsgInLoginUnwanted.h"


class MsgInDelGCMID : public IMsgInLoginUnwanted{
public:
    MsgInDelGCMID(char* msg, pugi::xml_document* doc);
    virtual ~MsgInDelGCMID();
    

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();
    static const std::string state;


    
private:

};

#endif	/* MSGINDELGCMID_H */

