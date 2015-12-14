#ifndef GATEINFO_H
#define	GATEINFO_H

#include "IRequestHandler.h"
#include "../lib/pugixml.hpp"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOUsers.h"
#include "DAO/DAOGateways.h"

class GatesGetRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        GatesGetRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOGateways & daoGateway, SessionsTable & sessionTable);
        virtual ~GatesGetRequestHandler();        
        virtual int getMsgAuthorization();        
        virtual std::string createResponseMsgOut();        
        static const std::string state;
    private:
        DAOGateways & _daoGateways;
};

#endif	/* GATEINFO_H */

