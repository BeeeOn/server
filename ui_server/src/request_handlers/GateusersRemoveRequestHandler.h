#ifndef MSGINDELCONACCOUNT_H
#define	MSGINDELCONACCOUNT_H

#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOUsersGateways.h"

class GateusersRemoveRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        GateusersRemoveRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOUsersGateways & daoUsersGateways, SessionsTable & sessionTable);
        virtual ~GateusersRemoveRequestHandler();       

        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        
        static const std::string state;
    private:
        DAOUsersGateways & _daoUsersGateways;    
};

#endif	/* MSGINDELCONACCOUNT_H */

