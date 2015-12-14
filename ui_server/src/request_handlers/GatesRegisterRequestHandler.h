#ifndef MSGINADDADAPTER_H
#define	MSGINADDADAPTER_H

#include "IRequestHandlerWithLoginRequired.h"
#include "../DAO/DAOGateways.h"
#include "../DAO/DAOUsersGateways.h"
#include "../DAO/DAOUsers.h"
#include "IRequestHandler.h"

class GatesRegisterRequestHandler
    :   public IRequestHandlerWithLoginRequired
    {
public:
    GatesRegisterRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOGateways & daoGateway, DAOUsersGateways & daoUsersGateway, SessionsTable & sessionTable);
    virtual ~GatesRegisterRequestHandler();
    virtual std::string createResponseMsgOut();
    static const std::string state;

private:
    DAOUsers & _daoUsers;
    DAOGateways & _daoGateways;
    DAOUsersGateways & _daoUsersGateways;
};

#endif	/* MSGINADDADAPTER_H */

