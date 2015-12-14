#ifndef MSGINCHANGECONACCOUNT_H
#define	MSGINCHANGECONACCOUNT_H

#include "IRequestHandler.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOUsersGateways.h"
#include "DAO/DAOUsers.h"

class GateusersUpdateRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired 
{
    public:
        GateusersUpdateRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOUsersGateways & daoUsersGateways, SessionsTable & sessionTable);
        virtual ~GateusersUpdateRequestHandler();

        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();

        static const std::string state;
    private:
        DAOUsersGateways & _daoUsersGateways;
};

#endif	/* MSGINCHANGECONACCOUNT_H */

