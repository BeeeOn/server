#ifndef MSGINSWITCH_H
#define	MSGINSWITCH_H

#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAODevices.h"
#include "AdaServerCommunicator.h"

class DevicesSetStateRequestHandler 
:   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        DevicesSetStateRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, AdaServerCommunicator & adaServerCom, SessionsTable & sessionTable);
        virtual ~DevicesSetStateRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
        
    private:
        AdaServerCommunicator & _adaServerCom;
};

#endif	/* MSGINSWITCH_H */

