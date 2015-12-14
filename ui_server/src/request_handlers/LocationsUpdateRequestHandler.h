#ifndef MSGINUPDATEROOMS_H
#define	MSGINUPDATEROOMS_H

#include "IRequestHandler.h"
#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "DAO/DAOLocations.h"
#include "DAO/DAOUsers.h"

class LocationsUpdateRequestHandler
    :	public IRequestHandlerWithLoginAndGateAccessRequired 
    {
    public:
        LocationsUpdateRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOLocations & daoLocations, SessionsTable & sessionTable);
        virtual ~LocationsUpdateRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;

    private:
        DAOLocations & _daoLocations;

};

#endif	/* MSGINUPDATEROOMS_H */

