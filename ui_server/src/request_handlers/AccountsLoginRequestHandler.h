#ifndef MSGINSIGNME_H
#define	MSGINSIGNME_H

#include "IRequestHandler.h"
#include "../DAO/DAOUsers.h"
#include "IRequestHandlerWithoutUac.h"
#include "SessionsTable.h"

class AccountsLoginRequestHandler
    :   public IRequestHandlerWithoutUac
{
    public:
        AccountsLoginRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, SessionsTable & sessionTable);
        virtual ~AccountsLoginRequestHandler(void);
        virtual std::string createResponseMsgOut();

        virtual int getMsgAuthorization();

        static const std::string state;
    private:
        DAOUsers & _daoUsers;
        SessionsTable & _sessionTable;
        };
        
#endif /* MSGINSIGNME_H */