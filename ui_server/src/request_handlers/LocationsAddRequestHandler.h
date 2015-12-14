#ifndef MSGINADDROOM_H
#define	MSGINADDROOM_H

#include "IRequestHandler.h"
#include "../lib/pugixml.hpp"
#include "IRequestHandlerWithLoginRequired.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "../DAO/DAOLocations.h"

class LocationsAddRequestHandler
    :   public IRequestHandlerWithLoginAndGateAccessRequired
{
    public:
        LocationsAddRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOLocations & daoLocations, SessionsTable & sessionTable);
        virtual ~LocationsAddRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;
    private:
        DAOLocations & _daoLocations;
    
};

#endif	/* MSGINADDROOM_H */

