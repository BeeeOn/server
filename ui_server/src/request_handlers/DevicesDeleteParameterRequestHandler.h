#ifndef DEVICESDELETEPARAMETERREQUESTHANDLER_H
#define DEVICESDELETEPARAMETERREQUESTHANDLER_H

#include "DAO/DAOUsers.h"
#include "DAO/DAODevicesParameters.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"

class DevicesDeleteParameterRequestHandler
    :    public IRequestHandlerWithLoginAndGateAccessRequired
{
        
    public:
        DevicesDeleteParameterRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevicesParameters & daoDevPar, SessionsTable & sessionTable);
        virtual ~DevicesDeleteParameterRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;

    private:
        DAODevicesParameters & _daoDevPar;

};

#endif /* DEVICESDELETEPARAMETERREQUESTHANDLER_H */

