#ifndef MSGINSIGNUP_H
#define	MSGINSIGNUP_H

#include "IRequestHandler.h"

#include "IRequestHandlerWithoutUac.h"
#include "IRequestHandlerWithLoginRequired.h"
#include "../DAO/DAOUsers.h"

class AccountsRegisterRequestHandler :
	public IRequestHandlerWithoutUac
{
    public:
        AccountsRegisterRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers);
        virtual ~AccountsRegisterRequestHandler(void);
        virtual std::string createResponseMsgOut();

        virtual int getMsgAuthorization();

        static const std::string state;
    private:
        DAOUsers & _daoUsers;
        };
        
#endif /* MSGINSIGNUP_H */