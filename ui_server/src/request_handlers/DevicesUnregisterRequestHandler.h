#ifndef MSGINDELDEVICE_H
#define	MSGINDELDEVICE_H

#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "AdaServerCommunicator.h"

class DevicesUnregitersRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        DevicesUnregitersRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, AdaServerCommunicator & adaServerCom, SessionsTable & sessionTable);
        virtual ~DevicesUnregitersRequestHandler();

        virtual int getMsgAuthorization();

        virtual std::string createResponseMsgOut();

        static const std::string state;
    private:
        AdaServerCommunicator & _adaServerCom;
        

};

#endif	/* MSGINDELDEVICE_H */

