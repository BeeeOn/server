#ifndef MSGINUPDATE_H
#define	MSGINUPDATE_H

#include "IMsgIn.h"
#include "DBConnector.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

class DevicesGet : public IMsgInLoginRequired
{
public:
    DevicesGet(pugi::xml_document* doc);
    virtual ~DevicesGet(void);
    
    virtual int getMsgAuthorization();
    
    virtual std::string createResponseMsgOut();
    
    static const std::string state;
};

#endif /* MSGINUPDATE_H */