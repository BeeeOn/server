#ifndef MSGINUPDATE_H
#define	MSGINUPDATE_H

#include "IRequestHandler.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAODevices.h"

class DevicesGetRequestHandler 
    :   public IRequestHandlerWithLoginRequired
{
    public:
        DevicesGetRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAODevices &daoDevices, SessionsTable & sessionTable);
        virtual ~DevicesGetRequestHandler(void);
        virtual std::string createResponseMsgOut();
        static const std::string state;
    private:
        DAODevices & _daoDevices;
};

#endif /* MSGINUPDATE_H */