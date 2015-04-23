/* 
 * File:   DAONotification.cpp
 * Author: Pavel
 * 
 * Created on 23. dubna 2015, 13:29
 */

#include "DAONotification.h"
#include "ServerException.h"
#include <postgresql/soci-postgresql.h>
using namespace std;
using namespace soci;

DAONotification::DAONotification() {
}

DAONotification::~DAONotification() {
}

DAONotification& DAONotification::getInstance(){
        static DAONotification instance;
        return instance;
}

std::string DAONotification::getXMLNotifications(int userId) {
    Logger::debug3()<<"DB:"<<"getNotifications"<<userId<<endl;
    try{
        soci::session sql(*_pool);
        string xml;
        sql << "select xmlagg( xmlelement(name notif, xmlattributes(message_id as mid, timestamp as time, level as type, read as read),"
                " text) ) "
                "from notifications where fk_user_id=:user_id",       
                use(userId, "user_id"), into(xml);
    
        return xml;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::error() << "Error: " << e.what() << endl;
        return 0;
    }
}

