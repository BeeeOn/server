#ifndef MSGINDELADAPTER_H
#define	MSGINDELADAPTER_H

#include "IRequestHandler.h"
#include "IRequestHandlerWithLoginRequired.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOGateways.h"
#include "DAO/DAOUsersGateways.h"
#include "DAO/DAOUsers.h"

class GatesUnregisterRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired 
{
    public:
        GatesUnregisterRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOGateways & daoGateways, DAOUsersGateways & daoUsersGateways, SessionsTable & sessionTable);
        virtual ~GatesUnregisterRequestHandler();

        virtual std::string createResponseMsgOut();
        virtual int getMsgAuthorization();
        static const std::string state;

    private:
        DAOUsersGateways & _daoUsersGateways;
        DAOGateways & _daoGateways;
};

#endif	/* MSGINDELADAPTER_H */

