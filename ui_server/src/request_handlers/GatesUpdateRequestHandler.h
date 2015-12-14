#ifndef MSGINSETGATE_H
#define	MSGINSETGATE_H

#include "IRequestHandlerWithLoginRequired.h"
#include "../../DAO/DAOGateways.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"


class GatesUpdateRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        GatesUpdateRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOGateways & daoGateways, SessionsTable & sessionTable);

        virtual ~GatesUpdateRequestHandler();

        virtual int getMsgAuthorization();

        virtual std::string createResponseMsgOut();

        static const std::string state;
    private:
        DAOGateways & _daoGateways;
};

#endif	/* MSGINSETGATE_H */

