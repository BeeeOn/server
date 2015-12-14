#ifndef MSGINDELROOM_H
#define	MSGINDELROOM_H

#include "IRequestHandler.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOLocations.h"

class LocationsDeleteRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        LocationsDeleteRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOLocations & daoLocations, SessionsTable & sessionTable);
        virtual ~LocationsDeleteRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
    private:
        DAOLocations & _daoLocations;
};

#endif	/* MSGINDELROOM_H */

