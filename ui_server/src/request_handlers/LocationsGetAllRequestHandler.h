#ifndef MSGINGETROOMS_H
#define	MSGINGETROOMS_H

#include "IRequestHandler.h"
#include "IRequestHandlerWithLoginRequired.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOLocations.h"

class LocationsGetAllRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        LocationsGetAllRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOLocations & daoLocations, SessionsTable & sessionTable);
        virtual ~LocationsGetAllRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;

    private:
        DAOLocations & _daoLocations;    
};

#endif	/* MSGINGETROOMS_H */

