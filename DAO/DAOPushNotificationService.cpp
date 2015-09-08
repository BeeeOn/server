/* 
 * File:   DAOPushNotificationService.cpp
 * Author: pavel
 * 
 * Created on 27. srpen 2015, 13:52
 */

#include "DAOPushNotificationService.h"

using namespace std;
using namespace soci;


const pushNotificationServiceColumns DAOPushNotificationService::col;
const std::string DAOPushNotificationService::tablePushNotifService = "push_notification_service";

DAOPushNotificationService::DAOPushNotificationService(){
}


DAOPushNotificationService::~DAOPushNotificationService() {
}

DAOPushNotificationService& DAOPushNotificationService::getInstance(){
        static DAOPushNotificationService instance;
        return instance;
}

int DAOPushNotificationService::insertPushNotification(int userId, string push_notif) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"set notif serviceID"<<push_notif<<endl;
    try{
        soci::session sql(*_pool);
        
       statement st = (sql.prepare << 
                "insert into "<< tablePushNotifService <<"(" <<col.userId << ", "<< col.serviceReference <<") values(:userId, :notif) ",
                use(userId, "userId"), use(push_notif, "notif")
             );
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}
int DAOPushNotificationService::deletepushNotification(int oldUserId, string notification) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"delGCMId"<<oldUserId<<endl;
    try{
        soci::session sql(*_pool);
        
        statement st = (sql.prepare << 
                "delete from "<< tablePushNotifService <<" where " <<col.userId << " = :user_id and "<< col.serviceReference <<" =:notif",
                use(oldUserId, "user_id"),use(notification,"notif")
             );
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}
