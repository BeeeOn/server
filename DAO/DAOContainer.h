#ifndef DAOCONTAINER_H
#define DAOCONTAINER_H

#include "DAOUsers.h"
#include "DAODevices.h"
#include "DAOLocations.h"
#include "DAONotification.h"
#include "DAOPushNotificationService.h"
#include "DAOUsersGateways.h"
#include "DAOlogs.h"
#include "DAODevicesParameters.h"

#include <string>

class DAOContainer 
{
public:
    DAOContainer(std::string dbConnectionString, int sessionPoolSize);
    virtual ~DAOContainer();
    
    DAODevices & getDaoDevices();
    DAOGateways & getDaoGateway();
    DAOLocations & getDaoLocations();
    DAOMobileDevices & getDaoMobileDevices();
    DAONotification & getDaoNotification();
    DAOPushNotificationService & getDaoPushNotification();
    DAOUsers & getDaoUsers();
    DAOUsersGateways & getDaoUsersGateway();
    DAOlogs & getDaoLogs();
    DAODevicesParameters & getDaoDevicesParameters();
    
private:
    DAODevices _daoDevices;
    DAOGateways _daoGateway;
    DAOLocations _daoLocations;
    DAOMobileDevices _daoMobileDevices;
    DAONotification _daoNotification;
    DAOPushNotificationService _daoPushNotification;
    DAOUsers _daoUsers;
    DAOUsersGateways _daoUsersGateway;
    DAOlogs _daoLogs;
    DAODevicesParameters _daoDevPar;
};

#endif /* DAOCONTAINER_H */

