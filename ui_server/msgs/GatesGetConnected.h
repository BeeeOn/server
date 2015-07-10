#ifndef MSGETADAPTERS_H
#define	MSGETADAPTERS_H

#include "IMsgIn.h"
#include "../DAO/DAOUsers.h"
#include "IMsgInLoginRequired.h"

class GatesGetConnected :
public IMsgInLoginRequired
{
    public:
        GatesGetConnected(pugi::xml_document* doc);
        virtual ~GatesGetConnected(void);
        
        virtual int getMsgAuthorization();
        
        virtual std::string createResponseMsgOut();
        
        static const std::string state;
};
#endif /* MSGETADAPTERS_H */
