#ifndef MSGINUPDATE_H
#define	MSGINUPDATE_H

#include "IMsgIn.h"
#include "DBConnector.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

using namespace std;
class MsgInUpdate : public IMsgInLoginRequired
{
public:
    MsgInUpdate(char* msg, pugi::xml_document* doc);
    virtual ~MsgInUpdate(void);
    
    virtual int getMsgAuthorization();
    
    virtual string createResponseMsgOut();
    
    static const std::string state;
};

#endif /* MSGINUPDATE_H */