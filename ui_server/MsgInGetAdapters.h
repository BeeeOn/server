#ifndef MSGETADAPTERS_H
#define	MSGETADAPTERS_H

#include "IMsgIn.h"
#include "DBConnector.h"
#include "IMsgInLoginRequired.h"

class MsgInGetAdapters :
public IMsgInLoginRequired
{
    public:
        MsgInGetAdapters(char* msg, pugi::xml_document* doc);
        virtual ~MsgInGetAdapters(void);
        
        virtual int getMsgAuthorization();
        
        virtual string createResponseMsgOut();
        
        static const std::string state;
};
#endif /* MSGETADAPTERS_H */