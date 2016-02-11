
#ifndef GATESSEARCHREQUESTHANDLER_H
#define GATESSEARCHREQUESTHANDLER_H

#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "AdaServerCommunicator.h"


class GatesSearchRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired 
{
    public:
        GatesSearchRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, AdaServerCommunicator & adaServerCom, SessionsTable & sessionTable);
        virtual ~GatesSearchRequestHandler();
        
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
        
    private:
        AdaServerCommunicator & _adaServerCom;

};

#endif /* GATESSEARCHREQUESTHANDLER_H */

