#ifndef MSGETADAPTERS_H
#define	MSGETADAPTERS_H

#include "IMsgIn.h"
#include "../DAO/DAOUsers.h"
#include "IMsgInLoginRequired.h"

class MsgInGetAdapters :
public IMsgInLoginRequired
{
    public:
        MsgInGetAdapters(pugi::xml_document* doc);
        virtual ~MsgInGetAdapters(void);
        
        virtual int getMsgAuthorization();
        
        virtual std::string createResponseMsgOut();
        
        static const std::string state;
};
#endif /* MSGETADAPTERS_H */
