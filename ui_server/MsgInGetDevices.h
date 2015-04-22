#ifndef MSGINUPDATE_H
#define	MSGINUPDATE_H

#include "IMsgIn.h"
#include "DBConnector.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

class MsgInGetDevs : public IMsgInLoginRequired
{
public:
    MsgInGetDevs(char* msg, pugi::xml_document* doc);
    virtual ~MsgInGetDevs(void);
    
    virtual int getMsgAuthorization();
    
    virtual std::string createResponseMsgOut();
    
    static const std::string state;
};

#endif /* MSGINUPDATE_H */