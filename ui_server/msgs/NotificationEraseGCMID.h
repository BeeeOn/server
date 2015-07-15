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


class NotificationEraseGCMID : public IMsgInFreeAccess{
public:
    NotificationEraseGCMID(pugi::xml_document* doc);
    virtual ~NotificationEraseGCMID();
    

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();
    static const std::string state;


    
private:

};

#endif	/* MSGINDELGCMID_H */

