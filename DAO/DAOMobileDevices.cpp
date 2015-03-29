/* 
 * File:   DAOMobileDevices.cpp
 * Author: pavel
 * 
 * Created on 29. březen 2015, 21:47
 */

#include "DAOMobileDevices.h"

using namespace std;
using namespace soci;

DAOMobileDevices::DAOMobileDevices(){
}


DAOMobileDevices::~DAOMobileDevices() {
}

DAOMobileDevices& DAOMobileDevices::getInstance(){
        static DAOMobileDevices instance;
        return instance;
}

int DAOMobileDevices::delGCMId(string oldUserId, string gcmid) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"delGCMId"<<oldUserId<<endl;
    try{
        soci::session sql(*_pool);
        
        statement st = (sql.prepare << 
                "update mobile_devices set push_notification = '' where fk_user_id = :user_id and push_notification=:gcmid",
                use(oldUserId, "user_id"),use(gcmid,"gcmid")
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

int DAOMobileDevices::setGCMId(string IHAtoken, string phoneid,int userId, string gcmid) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"setGCMId"<< "guser"<<userId<<"tok:"<<IHAtoken<<"gc"<<gcmid<<endl;
    try{
        soci::session sql(*_pool);
        
       statement st = (sql.prepare << 
                "update mobile_devices set push_notification = :gcmid where token = :ihaToken",
                use(IHAtoken, "ihaToken"), use(gcmid,"gcmid")
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

int DAOMobileDevices::upsertMobileDevice(MobileDevice mobile, User user){
    session sql(*_pool);
    Logger::db()<<"DB:"<<"upsertMobileDevice"<<endl;
    try
    {
            soci::session sql(*_pool);

            int newUserID;
            //d_mail TEXT, d_locale TEXT, d_ver BOOLEAN, d_name TEXT, d_g_name TEXT, d_f_name TEXT, d_link TEXT, d_picture TEXT, d_gender TEXT, d_g_loc TEXT, d_g_id TEXT
            
            sql << "select user_id from users where mail=:mail",use(user.mail),into(newUserID);
            
            //bon_token TEXT, d_mobile_id TEXT,d_type TEXT, d_locale TEXT, d_push_n TEXT, d_uid integer
            sql << "select upsert_mobile_device(:bt, :mobileID, :type, :loc, :pushN, :userID)",
                    use(mobile.token), use(mobile.mobile_id), use(mobile.type), use(mobile.locale),use(mobile.push_notification),use(newUserID);
            return 1;
    }
    catch (soci::postgresql_soci_error& e)
    {
                Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            return 0;
    }
}
