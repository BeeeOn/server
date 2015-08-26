/* 
 * File:   MsgInDelAdapter.h
 * Author: Pavel
 *
 * Created on 7. dubna 2015, 9:09
 */

#ifndef MSGINDELADAPTER_H
#define	MSGINDELADAPTER_H

#include "IMsgIn.h"
#include "IMsgInLoginRequired.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

class GateDelete : public IMsgInLoginRequired {
public:
    GateDelete(pugi::xml_document* doc);
    virtual ~GateDelete();
    
    virtual std::string createResponseMsgOut();
    virtual int getMsgAuthorization();
    static const std::string state;
 
private:

};

#endif	/* MSGINDELADAPTER_H */

