#ifndef DEVICESUPDATEPARAMETERREQUESTHANDLER_H
#define DEVICESUPDATEPARAMETERREQUESTHANDLER_H

#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOUsers.h"
#include "DAO/DAODevicesParameters.h"


class DevicesUpdateParameterRequestHandler
    :    public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        DevicesUpdateParameterRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevicesParameters & daoDevPar, SessionsTable & sessionTable);
        virtual ~DevicesUpdateParameterRequestHandler();  
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
        
    private:
        DAODevicesParameters & _daoDevPar;

};

#endif /* DEVICESUPDATEPARAMETERREQUESTHANDLER_H */

