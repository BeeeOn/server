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
        indicator ind;
        string xml;
        sql << "select xmlagg( xmlelement(name notif, xmlattributes(name as name, message_id as mid, timestamp as time, level as type, read::int as read),"
                " text) ) "
                "from notifications where fk_user_id=:user_id",       
                use(userId, "user_id"), into(xml,ind);
        return xml;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::error() << "Error: " << e.what() << endl;
        return 0;
    }
}

int DAONotification::setReaded(int notif) {
    Logger::debug3()<<"DB:"<<"readednotif"<<endl;
    try{
        soci::session sql(*_pool);
        sql << "update notifications set read = true where message_id = :not_id",       
                use(notif, "not_id");
        return 1;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::error() << "Error: " << e.what() << endl;
        return 0;
    }
}

