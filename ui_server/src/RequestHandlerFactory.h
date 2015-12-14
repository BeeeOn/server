#ifndef MSGFACTORY_H
#define	MSGFACTORY_H

#include <string>
#include "request_handlers/IRequestHandler.h"

#include "DAO/DAOUsers.h"
#include "DAO/DAODevices.h"
#include "DAO/DAOlogs.h"
#include "DAO/DAOLocations.h"
#include "DAO/DAOPushNotificationService.h"
#include "DAO/DAONotification.h"
#include "DAO/DAOUsersGateways.h"
#include "DAO/DAOMobileDevices.h"
#include "DAO/DAOContainer.h"
#include "AdaServerCommunicator.h"
#include "SessionsTable.h"

class IRequestHandler;

class RequestHandlerFactory
{
    public:
        RequestHandlerFactory(DAOContainer & daoContainer, AdaServerCommunicator & adaServerCom, SessionsTable & sessionTable);     
	~RequestHandlerFactory(void);
	std::shared_ptr<IRequestHandler> createMsg(const char* msg);   
    private:        
        DAOUsers & _daoUsers;
        DAOGateways & _daoGateways;
        DAODevices & _daoDevices;
        DAOLocations & _daoLocations;
        DAOMobileDevices & _daoMobileDevices;        
        DAOUsersGateways & _daoUsersGateways;
        DAOlogs & _daoLogs;
        DAOPushNotificationService & _daoPushNofitication;
        DAONotification & _daoNotification;
        
        AdaServerCommunicator & _adaServerCom;
        SessionsTable &_sessionTable;
};

#endif /* MSGFACTORY_H */