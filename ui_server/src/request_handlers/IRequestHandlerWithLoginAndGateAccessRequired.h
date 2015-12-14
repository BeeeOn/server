#ifndef IMSGINLOGINANDGATEACCESSREQUIRED_H
#define	IMSGINLOGINANDGATEACCESSREQUIRED_H

#include "IRequestHandlerWithLoginRequired.h"
#include "DAO/DAOUsers.h"

class IRequestHandlerWithLoginAndGateAccessRequired 
    :   public IRequestHandlerWithLoginRequired
{
    public:
        IRequestHandlerWithLoginAndGateAccessRequired(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, SessionsTable & sessionTable);
        virtual ~IRequestHandlerWithLoginAndGateAccessRequired();    
        virtual enumAccessStatus checkAccess();
        virtual int getMsgAuthorization() = 0;    
        std::string _role;
        gatewayId_64b _gatewayId;   
    protected:
        DAOUsers & _daoUsers;
};

#endif	/* IMSGINLOGINANDGATEACCESSREQUIRED_H */

