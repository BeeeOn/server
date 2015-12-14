#ifndef MSGINGETUSERINFO_H
#define	MSGINGETUSERINFO_H

#include "IRequestHandlerWithLoginRequired.h"
#include "DAO/DAOUsers.h"

class AccountGetProfileRequestHandler
    :   public IRequestHandlerWithLoginRequired 
{
    public:
        AccountGetProfileRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, SessionsTable & sessionTable);
        virtual ~AccountGetProfileRequestHandler();
        virtual std::string createResponseMsgOut();
        virtual int getMsgAuthorization();
        static const std::string state;
    private:
        DAOUsers & _daoUsers;
            
};

#endif	/* MSGINGETUSERINFO_H */

