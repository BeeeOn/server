#ifndef MSGINGETNEWDEVICES_H
#define	MSGINGETNEWDEVICES_H

#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "../DAO/DAODevices.h"

class DevicesGetNewRequestHandler 
    :   public IRequestHandlerWithLoginAndGateAccessRequired 
{
    public:
        DevicesGetNewRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAODevices & daoDevices, SessionsTable & sessionTable);
        virtual ~DevicesGetNewRequestHandler();
        virtual int getMsgAuthorization();
        virtual std::string createResponseMsgOut();
        static const std::string state;

    private:
        DAODevices & _daoDevices;

};

#endif	/* MSGINGETNEWDEVICES_H */

