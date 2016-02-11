#ifndef DEVICESGETPARAMETERREQUESTHANDLER_H
#define DEVICESGETPARAMETERREQUESTHANDLER_H

#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOUsers.h"
#include "DAO/DAODevicesParameters.h"

class DevicesGetParameterRequestHandler 
    :    public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        DevicesGetParameterRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevicesParameters & daoDevPar, SessionsTable & sessionTable);
        virtual ~DevicesGetParameterRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
    private:
        DAODevicesParameters & _daoDevPar;

};

#endif /* DEVICESGETPARAMETERREQUESTHANDLER_H */

