#ifndef MSGINGETCONACCOUNT_H
#define	MSGINGETCONACCOUNT_H

#include "IRequestHandler.h"
#include "IRequestHandlerWithLoginRequired.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOUsersGateways.h"

class GateusersGetAllRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        GateusersGetAllRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOUsersGateways & daoUsersGateways, SessionsTable & sessionTable);
        virtual ~GateusersGetAllRequestHandler();
        virtual std::string createResponseMsgOut();

        virtual int getMsgAuthorization();
        static const std::string state;
    private:
        DAOUsersGateways & _daoUsersGateways;
};

#endif	/* MSGINGETCONACCOUNT_H */

