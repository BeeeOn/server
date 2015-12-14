#ifndef MSGINGETXML_H
#define	MSGINGETXML_H

#include "IRequestHandler.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "../DAO/DAODevices.h"

#include <string>

class DevicesGetAllRequestHandler 
    :   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        DevicesGetAllRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevices & daoDevices, SessionsTable & sessionTable);
        virtual ~DevicesGetAllRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
        
    private:
        DAODevices & _daoDevices;
};

#endif	/* MSGINGETXML_H */

