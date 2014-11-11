#ifndef MSGINUPDATE_H
#define	MSGINUPDATE_H

#include "IMsgIn.h"
#include "DBConnector.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

using namespace std;
class MsgInGetDevs : public IMsgInLoginRequired
{
public:
    MsgInGetDevs(char* msg, pugi::xml_document* doc);
    virtual ~MsgInGetDevs(void);
    
    virtual int getMsgAuthorization();
    
    virtual string createResponseMsgOut();
    
    static const std::string state;
};

#endif /* MSGINUPDATE_H */