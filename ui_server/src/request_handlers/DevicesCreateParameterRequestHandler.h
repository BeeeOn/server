#ifndef DEVICESCREATEPARAMETERREQUESTHANDLER_H
#define DEVICESCREATEPARAMETERREQUESTHANDLER_H

#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOUsers.h"
#include "DAO/DAODevicesParameters.h"

class DevicesCreateParameterRequestHandler
    :    public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        DevicesCreateParameterRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevicesParameters & daoDevPar, SessionsTable & sessionTable);
        virtual ~DevicesCreateParameterRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
    private:
        DAODevicesParameters & _daoDevPar;

};

#endif /* DEVICESCREATEPARAMETERREQUESTHANDLER_H */

