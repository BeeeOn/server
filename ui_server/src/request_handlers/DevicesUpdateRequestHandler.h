#ifndef MSGINPARTIALL_H
#define	MSGINPARTIALL_H

#include "../lib/pugixml.hpp"
#include <string>
#include <iostream>
#include "IRequestHandler.h"
#include "IRequestHandlerWithLoginRequired.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "../DAO/DAODevices.h"

class DevicesUpdateRequestHandler 
    :   public IRequestHandlerWithLoginAndGateAccessRequired 
{
    public:
        DevicesUpdateRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevices & daoDevices, SessionsTable & sessionTable);
        virtual ~DevicesUpdateRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
    private:
        DAODevices & _daoDevices;
};

#endif	/* MSGINPARTIALL_H */

