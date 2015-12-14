#ifndef MSGINLOGNAME_H
#define	MSGINLOGNAME_H

#include "IRequestHandler.h"
#include "../lib/pugixml.hpp"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOlogs.h"

class DevicesGetLogRequestHandler 
    :   public IRequestHandlerWithLoginAndGateAccessRequired 
{
    public:
        DevicesGetLogRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOlogs & daoLogs, SessionsTable & sessionTable);
        virtual ~DevicesGetLogRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
    private:
        DAOlogs & _daoLogs;
};

#endif	/* MSGINLOGNAME_H */

