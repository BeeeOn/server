#include "DAOContainer.h"

using namespace std;
using namespace soci;

DAOContainer::DAOContainer(string dbConnectionString, int sessionPoolSize) 
{
    try
    {      
        Logger::debug()<< "setting connections to DB..."<< endl;
                
        soci::connection_pool *pool = new soci::connection_pool(sessionPoolSize);
        for (int i = 0; i != sessionPoolSize; ++i)
        {
            soci::session & sql = pool->at(i);
            sql.open(soci::postgresql, dbConnectionString);
        }   
        
        _daoUsers.setPool(pool);
        _daoGateway.setPool(pool);
        _daoDevices.setPool(pool);
        _daoLocations.setPool(pool);
        _daoMobileDevices.setPool(pool);
        _daoUsersGateway.setPool(pool);
        _daoLogs.setPool(pool);
        _daoPushNotification.setPool(pool);
        _daoNotification.setPool(pool);
        
        Logger::debug()<< "connection to DB is set"<< endl;
        
        {
            session sql(soci::postgresql, dbConnectionString);
            string dbUser;
            sql << "select * from user", into(dbUser);
            Logger::debug() << "DB user is: " << dbUser << endl;
        }
        
    }
    catch (soci::soci_error const & e)
    {
        Logger::fatal()<< "DB error (soci), probably cant set connection, more:" << e.what()<< endl;
        throw;
    }
}

DAOContainer::~DAOContainer() 
{
}

DAODevices & DAOContainer::getDaoDevices() 
{
    return _daoDevices;
}

DAOGateways & DAOContainer::getDaoGateway() 
{
    return _daoGateway;
}

DAOLocations & DAOContainer::getDaoLocations() 
{
    return _daoLocations;
}

DAOlogs & DAOContainer::getDaoLogs() 
{
    return _daoLogs;
}

DAOMobileDevices & DAOContainer::getDaoMobileDevices() 
{
    return _daoMobileDevices;
}

DAONotification & DAOContainer::getDaoNotification() 
{
    return _daoNotification;
}

DAOPushNotificationService & DAOContainer::getDaoPushNotification() 
{
    return _daoPushNotification;
}

DAOUsers & DAOContainer::getDaoUsers() 
{
    return _daoUsers;
}

DAOUsersGateways & DAOContainer::getDaoUsersGateway() 
{
    return _daoUsersGateway;
}


