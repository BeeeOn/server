#ifndef MSGINADAPTERLISTEN_H
#define	MSGINADAPTERLISTEN_H

#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "SocketClient.h"
#include "AdaServerCommunicator.h"

class GatesStartListenRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        GatesStartListenRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, AdaServerCommunicator & adaServerCom, SessionsTable & sessionTable);
        virtual ~GatesStartListenRequestHandler();
        
        virtual int getMsgAuthorization();

        virtual std::string createResponseMsgOut();

        static const std::string state;
    private:
        AdaServerCommunicator & _adaServerCom;
            
};

#endif	/* MSGINADAPTERLISTEN_H */

