#ifndef MSGINADDCONACCOUNT_H
#define	MSGINADDCONACCOUNT_H

#include "IRequestHandler.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOUsersGateways.h"

class GateusersInviteRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        GateusersInviteRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOUsersGateways & daoUsersGateways, SessionsTable & sessionTable);
        virtual ~GateusersInviteRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();

        static const std::string state;
    private:
        DAOUsersGateways & _daoUsersGateways;
};

#endif	/* MSGINADDCONACCOUNT_H */

